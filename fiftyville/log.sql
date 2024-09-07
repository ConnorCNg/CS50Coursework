-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Getting format of street for eval
SELECT street FROM crime_scene_reports LIMIT 5;

-- Starting with crime scene reports on date and street
SELECT description
FROM crime_scene_reports
WHERE year = 2023 AND month = 7 AND day = 28 AND street = 'Humphrey Street';

-- 2 Crime Descriptions:
-- 1. Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.
-- 2. Littering took place at 16:36. No known witnesses.

-- 1st description mentions 3 witnesses and the bakery. Will the interviews and bakery security logs tables per .schema

-- Interviews transcripts
SELECT transcript
FROM interviews
WHERE year = 2023 AND month = 7 AND day = 28;

-- Guessing it's these 3:
-- 1. Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-- 2. I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
-- 3. As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Next steps:
-- 1. Check bakery security footage for cars between 10:15 - 10:25AM
-- 2. Look into ATM drawing on Leggett street before 10:15AM on the same day (ATM transactions table)
-- 3. Check phone calls table for short calls soon after 10:15AM same day. Check plane ticket purchases shortly after same day. Check flights next day

-- Get names of interviewees just in case
SELECT name, transcript
FROM interviews
WHERE year = 2023 AND month = 7 AND day = 28;

-- Ruth, Eugene, Raymond

-- Guessing I'm going to have to triangulate security log > license plate > people WITH flight > passengers WITH atm transactions > bank accounts > people for the main perp so just goign to do top to bottom

-- Getting people with license plates for cars seen at bakery between 10:15 - 10:25AM on July 28, 2023 inclusive. Get id incase people with same name
SELECT id, name
FROM people
WHERE license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2023 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25
)
ORDER BY name;

-- Got multiple names: Barry, Bruce, Diana, Iman, Kelsey, Luca, Sofia, Vanessa

-- Getting format of atm_location
SELECT atm_location FROM atm_transactions LIMIT 5;

-- Getting info of ATM drawing on Leggett street before 10:15AM on the same day (ATM transactions table)
SELECT id, name
FROM people
WHERE id IN (
    SELECT person_id
    FROM bank_accounts
    where account_number IN (
        SELECT account_number
        FROM atm_transactions
        WHERE year = 2023 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street'
    )
)
ORDER BY name;

-- This gave Benista, Brooke, Bruce, Diana, Iman, Kaelyn, Kenny, Luca, Taylor

-- Finding format of caller amd receiver in phone_calls for joining
SELECT caller from phone_calls LIMIT 5
SELECT receiver from phone_calls LIMIT 5

-- Make sure that matches the format in people for phone_number
SELECT phone_number from people LIMIT 5;

-- Check phone calls table for short calls soon after 10:15AM same day. Caller gives theif
SELECT id, name, phone_number
FROM people
WHERE people.phone_number IN (
    SELECT caller
    FROM phone_calls
    WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 60
)
ORDER by name;

-- Gave Benista, Bruce, Carina, Diana, Kelsey, Kenny, Sofia, Taylor

-- Put all 3 queries above together since they all end up in the people table:
SELECT id, name
FROM people
WHERE license_plate IN (
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2023 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25
)
AND id IN (
    SELECT person_id
    FROM bank_accounts
    where account_number IN (
        SELECT account_number
        FROM atm_transactions
        WHERE year = 2023 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street'
    )
)
AND people.phone_number IN (
    SELECT caller
    FROM phone_calls
    WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 60
)
ORDER BY name;

-- Results in Bruce 686048 and Diana 514354

-- Get the corresponding receiver for the phone calls for the 2 above.
SELECT id, name
FROM people
WHERE phone_number IN (
    SELECT receiver
    FROM phone_calls
    WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 60
    AND caller IN (
        SELECT phone_number
        FROM people
        WHERE name = 'Bruce' OR name = 'Diana'
    )
);

--  So Philip 847116 or Robin 864400 is the accomplice

-- Doublechecking based on phone numbers which receiver corresponds to which caller
SELECT caller, receiver
FROM phone_calls
WHERE year = 2023 AND month = 7 AND day = 28 AND duration < 60;

-- Match with people table
SELECT name, phone_number
FROM people
WHERE name = 'Bruce' OR name = 'Diana' OR name = 'Philip' OR name = 'Robin';

-- Diana with Philip and Bruce with Robin

-- Going to see who was on the earliest flight out of fiftyville

-- Getting the earliest times for flights out of Fiftyville on July 29, 2023
SELECT id, hour, minute, year, month, day
FROM flights
WHERE origin_airport_id = (
    SELECT id
    FROM airports
    WHERE city = 'Fiftyville'
)
ORDER BY day ASC, hour ASC, minute ASC;

-- earliest flight on the 29th is 8:30AM

-- people.passport_number > passengers.flight_id > flights.origin_airport = Fiftyville and flights time/day is next day and earliest
SELECT name
FROM people
WHERE passport_number IN (
    SELECT passport_number
    FROM passengers
    WHERE flight_id IN (
        SELECT id
        FROM flights
        WHERE year = 2023 AND month = 7 AND day = 29 AND hour = 8 AND minute = 20
        AND origin_airport_id = (
            SELECT id
            FROM airports
            WHERE city = 'Fiftyville'
        )
    )
)
ORDER by NAME;

-- Bruce is on the list so Robin is the accomplice

-- Getting the destination of the flight
SELECT destination_airport_id
FROM flights
WHERE origin_airport_id = (
    SELECT id
    FROM airports
    WHERE city = 'Fiftyville'
)
AND year = 2023 AND month = 7 AND day = 29 AND hour = 8 AND minute = 20
AND id in (
    SELECT flight_id
    FROM passengers
    WHERE passport_number = (
        SELECT passport_number
        FROM people
        WHERE name = 'Bruce'
    )
);

-- Got 4
-- Guess I didn't really need the Bruce criteria since there's only one flight at that date/time

-- Get the name of the city of the airport
SELECT city
FROM airports
WHERE id = 4;

--New York City

--The THIEF is:Bruce
--The city the thief ESCAPED TO:New York City
--The ACCOMPLICE is:Robin
