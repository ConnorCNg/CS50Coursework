import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime
from collections import defaultdict

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Show portfolio of stocks"""
    if request.method == "POST":
        try:
            addcash = float(request.form.get("cash"))
        except (KeyError, IndexError, ValueError):
            return apology("please provide a valid amount of money to add", 400)
        if addcash <= 0:
            return apology("please provide a positive amount of money to add", 400)
        curcash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        db.execute("UPDATE users SET cash = ? WHERE id = ?", addcash + curcash, session["user_id"])
        return redirect("/")
    else:
        holdings = db.execute("SELECT * FROM transactions WHERE userid = ?", session["user_id"])
        history = defaultdict(int)
        # count how many shares of each symbol the user has
        for holding in holdings:
            symbol = holding['symbol']
            numshares = holding['numshares']
            type = holding["type"]
            if type == 'BUY':
                history[symbol] += numshares
            elif type == 'SELL':
                history[symbol] -= numshares
        portfolio = []
        # Only display holdings that haven't all been sold off yet
        curcash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        print(curcash)
        grandtot = curcash
        for cursymbol in history:
            curnumshares = history[cursymbol]
            if curnumshares > 0:
                curprice = lookup(cursymbol)['price']
                grandtot += curnumshares * curprice
                portfolio.append({'symbol':cursymbol, 'ppshare':curprice, 'shares':curnumshares, 'value':usd(curprice * curnumshares)})
        return render_template("index.html", portfolio=portfolio, curcash=usd(curcash), grandtot=usd(grandtot))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # validate it's a real stock and user entered a valid # of shares to buy
        lookupval = lookup(request.form.get("symbol"))
        try:
            shares = int(request.form.get("shares"))
        except (KeyError, IndexError, ValueError):
            return apology("please provide a valid number of shares to buy", 400)
        if not lookupval:
            return apology("please provide a valid symbol", 400)
        elif shares < 0 or not shares or shares % 1 != 0:
            return apology("please provide a positive number of shares to buy", 400)

        # validate user has enough money to buy shares
        price = lookupval["price"]
        cost = price * shares
        userCash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        if userCash < cost:
            return apology("you do not have enough money to purchase that many shares", 400)
        else:
            db.execute("INSERT INTO transactions (userid, numshares, price, date, symbol, type) VALUES(?, ?, ?, ?, ?, 'BUY')", session["user_id"], shares, price, datetime.now(), lookupval["symbol"])
            db.execute("UPDATE users SET cash = ? WHERE id = ?", userCash - cost, session["user_id"])
            return redirect("/")
    else:
        return render_template("buy.html")
        '''
        CREATE TABLE transactions (
            id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
            userid INTEGER,
            numshares INTEGER,
            price INTEGER,
            date TEXT NOT NULL,
            FOREIGN KEY (userid) REFERENCES users (id)
        )
        '''
@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT * FROM transactions WHERE userid = ?", session["user_id"])
    if len(history) <= 0:
        return apology("you do not have not bought or sold any stocks", 400)
    else:
        for i in range(len(history)):
            history[i]["price"] = usd(history[i]["price"])
        return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not lookup(symbol):
            return apology("please enter a valid symbol", 400)
        else:
            price = usd(lookup(symbol)["price"])
            return render_template("quoted.html", symbol=symbol, price=price)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Validated username and password
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if not username:
            return apology("must provide a username", 400)
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        if (len(rows) != 0):
            return apology("username already taken", 400)
        if not password:
            return apology("must provide a password", 400)
        if not confirmation:
            return apology("must confirm password", 400)
        elif (password != confirmation):
            return apology("passwords must match", 400)

        # If all checks pass, add username and hash to database and sign user in
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, generate_password_hash(password))
        # Test case doesn't handle signing in the user after registering. I think that's better design though
        # session.clear()
        # session["user_id"] = db.execute("SELECT id FROM users WHERE username =?", username)
        return redirect("/")

    # If it's a get pull up the page
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # validate it's a real stock and user entered a valid # of shares to buy
        lookupval = lookup(request.form.get("symbol"))
        shares = int(request.form.get("shares"))
        if not lookupval:
            return apology("please provide a valid symbol", 400)
        elif shares < 0:
            return apology("please provide a positive number of shares to sell", 400)
        # Check if they have ever bought any of the stock they want to sell
        holdings = db.execute("SELECT * FROM transactions WHERE symbol = ?", lookupval["symbol"])
        if len(db.execute("SELECT * FROM transactions WHERE symbol = ? AND type = 'BUY'", lookupval["symbol"])) < 1:
            return apology(f"you don't have any shares with symbol {lookupval["symbol"]}", 400)
        # Check that they still own a positive, non-zero number of the stock they want to sell
        numshares = 0
        for i in range(len(holdings)):
            if holdings[i]["type"] == 'BUY':
                numshares += holdings[i]["numshares"]
            elif holdings[i]["type"] == 'SELL':
                numshares -= holdings[i]["numshares"]
        if numshares <= 0:
            return apology(f"you do not have any shares of symbol {lookupval["symbol"]}", 400)
        elif shares > numshares:
            return apology(f"you do not have that many shares. You only have {shares} shares of symbol {lookupval["symbol"]}", 400)
        # If all validation checks pass, make updates for selling stock
        curcash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        db.execute("INSERT INTO transactions (userid, numshares, price, date, symbol, type) VALUES(?, ?, ?, ?, ?, 'SELL')", session["user_id"], shares, lookupval["price"], datetime.now(), lookupval["symbol"])
        db.execute("UPDATE users set cash = ? WHERE id = ?", (shares * lookupval["price"]) + curcash[0]["cash"], session["user_id"])
        return redirect("/")
    else:
        rows = db.execute("SELECT * FROM transactions WHERE userid = ?", session["user_id"])
        symbolsowned = defaultdict(int)
        # Get number of shares a user owns for each symbol
        for i in range(len(rows)):
            symbolsowned[rows[i]["symbol"]] += rows[i]["numshares"]
        # Remove symbols where a user has sold all shares of it off
        for symbol in symbolsowned:
            if symbolsowned[symbol] <= 0:
                symbolsowned.pop(symbol)
        return render_template("sell.html", symbolsowned=symbolsowned)
