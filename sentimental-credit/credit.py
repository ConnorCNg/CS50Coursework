from cs50 import get_int
# from luhn import verify

'''
def get_int(prompt):
    try:
        return int(input(prompt))
    except ValueError:
        return False
'''


def verify(n):
    n = str(n)
    leng = len(n)
    sum = 0
    for i in range(leng - 2, -1, -2):
        multn = str(2 * int(n[i]))
        for j in range(0, len(multn)):
            sum += int(multn[j])
    for a in range(leng - 1, -1, -2):
        sum += int(n[a])
    return ((sum % 10) == 0)


def valid(cnum):
    if (cnum < 4000000000000) or (cnum >= 5600000000000000):
        return "INVALID"
    cnum = str(cnum)
    leng = len(cnum)
    prefix = cnum[0:2]
    if (leng == 15) and ((prefix == "34") or (prefix == "37")) and verify(cnum):
        return "AMEX"
    elif (leng == 16) and (51 <= int(prefix) <= 55) and verify(cnum):
        return "MASTERCARD"
    elif ((leng == 13) or (leng == 16)) and (40 <= int(prefix) <= 49) and verify(cnum):
        return "VISA"
    else:
        return "INVALID"


def main():
    cnum = get_int("Credit Card Number: ")
    print(valid(cnum))


main()
