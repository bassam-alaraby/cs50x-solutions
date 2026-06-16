-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Start with the description of the crime day (July 28, 2025)
SELECT description
FROM crime_scene_reports
WHERE year = 2025 AND month = 7 AND day = 28
AND street = "Humphrey Street";
-- Theft took place at 10:15am at the Humphrey Street bakery.
-- Interviewees mentions the bakery.
-- Littering took place at 16:36.


-- Check interviews on the day of the theft for clues
SELECT name, transcript
FROM interviews
WHERE year = 2025 AND month = 7 AND day = 28;
-- Ruth: Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-- Eugene: I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
-- Raymond: As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.


-- Look for cars leaving bakery parking lot around theft time
SELECT *
FROM bakery_security_logs
WHERE year = 2025 AND month = 7 AND day = 28
AND hour = 10
AND minute >= 15 AND minute <= 25
AND activity = "exit";


-- Get names of people who left the bakery around the theft time
SELECT *
FROM people
WHERE license_plate IN
(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2025 AND month = 7 AND day = 28
    AND hour = 10
    AND minute >= 15 AND minute <= 25
    AND activity = "exit"
);
-- Suspects:
--+--------+---------+----------------+-----------------+---------------+
--|   id   |  name   |  phone_number  | passport_number | license_plate |
--+--------+---------+----------------+-----------------+---------------+
--| 221103 | Vanessa | (725) 555-4692 | 2963008352      | 5P2BI95       |
--| 243696 | Barry   | (301) 555-4174 | 7526138472      | 6P58WS2       |
--| 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
--| 398010 | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
--| 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
--| 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
--| 560886 | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
--| 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
--+--------+---------+----------------+-----------------+---------------+


-- Check ATM withdrawals on Leggett Street that morning
SELECT *
FROM atm_transactions
WHERE year = 2025 AND month = 7 AND day = 28
AND atm_location = "Leggett Street"
AND transaction_type = "withdraw";


-- Get names of people who withdraw from ATM on Leggett Street that morning
SELECT *
FROM people
WHERE id IN
(
    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN
    (
        SELECT account_number
        FROM atm_transactions
        WHERE year = 2025 AND month = 7 AND day = 28
        AND atm_location = "Leggett Street"
        AND transaction_type = "withdraw"
    )
);
-- Names:
--+--------+---------+----------------+-----------------+---------------+
--|   id   |  name   |  phone_number  | passport_number | license_plate |
--+--------+---------+----------------+-----------------+---------------+
--| 395717 | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       |
--| 396669 | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
--| 438727 | Benista | (338) 555-6650 | 9586786673      | 8X428L0       |
--| 449774 | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
--| 458378 | Brooke  | (122) 555-4581 | 4408372428      | QX4YZN3       |
--| 467400 | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
--| 514354 | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
--| 686048 | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
--+--------+---------+----------------+-----------------+---------------+


-- Suspects:
SELECT *
FROM people
WHERE id IN
(
    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN
    (
        SELECT account_number
        FROM atm_transactions
        WHERE year = 2025 AND month = 7 AND day = 28
        AND atm_location = "Leggett Street"
        AND transaction_type = "withdraw"
    )
)
AND license_plate IN
(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2025 AND month = 7 AND day = 28
    AND hour = 10
    AND minute >= 15 AND minute <= 25
    AND activity = "exit"
);
--+--------+-------+----------------+-----------------+---------------+
--|   id   | name  |  phone_number  | passport_number | license_plate |
--+--------+-------+----------------+-----------------+---------------+
--| 396669 | Iman  | (829) 555-5269 | 7049073643      | L93JTIZ       |
--| 467400 | Luca  | (389) 555-5198 | 8496433585      | 4328GD8       |
--| 514354 | Diana | (770) 555-1861 | 3592750733      | 322W7JE       |
--| 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
--+--------+-------+----------------+-----------------+---------------+


-- -- Calls less than 60 seconds on July 28
SELECT *
FROM phone_calls
WHERE year = 2025 AND month = 7 AND day = 28
AND duration < 60;
--+-----+----------------+----------------+------+-------+-----+----------+
--| id  |     caller     |    receiver    | year | month | day | duration |
--+-----+----------------+----------------+------+-------+-----+----------+
--| 221 | (130) 555-0289 | (996) 555-8899 | 2025 | 7     | 28  | 51       |
--| 224 | (499) 555-9472 | (892) 555-8872 | 2025 | 7     | 28  | 36       |
--| 233 | (367) 555-5533 | (375) 555-8161 | 2025 | 7     | 28  | 45       |
--| 251 | (499) 555-9472 | (717) 555-1342 | 2025 | 7     | 28  | 50       |
--| 254 | (286) 555-6063 | (676) 555-6554 | 2025 | 7     | 28  | 43       |
--| 255 | (770) 555-1861 | (725) 555-3243 | 2025 | 7     | 28  | 49       |
--| 261 | (031) 555-6622 | (910) 555-3251 | 2025 | 7     | 28  | 38       |
--| 279 | (826) 555-1652 | (066) 555-9701 | 2025 | 7     | 28  | 55       |
--| 281 | (338) 555-6650 | (704) 555-2131 | 2025 | 7     | 28  | 54       |
--+-----+----------------+----------------+------+-------+-----+----------+


-- Name, Phone_Number of caller
SELECT name, phone_number
FROM people
WHERE phone_number IN
(
    SELECT caller
    FROM phone_calls
    WHERE year = 2025 AND month = 7 AND day = 28
    AND duration < 60
);
--+---------+----------------+
--|  name   |  phone_number  |
--+---------+----------------+
--| Kenny   | (826) 555-1652 |
--| Sofia   | (130) 555-0289 |
--| Benista | (338) 555-6650 |
--| Taylor  | (286) 555-6063 |
--| Diana   | (770) 555-1861 |
--| Kelsey  | (499) 555-9472 |
--| Bruce   | (367) 555-5533 |
--| Carina  | (031) 555-6622 |
--+---------+----------------+

-- Suspects:
SELECT *
FROM people
WHERE id IN
(
    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN
    (
        SELECT account_number
        FROM atm_transactions
        WHERE year = 2025 AND month = 7 AND day = 28
        AND atm_location = "Leggett Street"
        AND transaction_type = "withdraw"
    )
)
AND license_plate IN
(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2025 AND month = 7 AND day = 28
    AND hour = 10
    AND minute >= 15 AND minute <= 25
    AND activity = "exit"
)
AND phone_number IN
(
    SELECT caller
    FROM phone_calls
    WHERE year = 2025 AND month = 7 AND day = 28
    AND duration < 60
);
--+--------+-------+----------------+-----------------+---------------+
--|   id   | name  |  phone_number  | passport_number | license_plate |
--+--------+-------+----------------+-----------------+---------------+
--| 514354 | Diana | (770) 555-1861 | 3592750733      | 322W7JE       |
--| 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
--+--------+-------+----------------+-----------------+---------------+


-- Earliest flight on July 29
SELECT *
FROM flights
WHERE year = 2025 AND month = 7 AND day = 29
ORDER BY hour, minute
LIMIT 1;
--+----+-------------------+------------------------+------+-------+-----+------+--------+
--| id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
--+----+-------------------+------------------------+------+-------+-----+------+--------+
--| 36 | 8                 | 4                      | 2025 | 7     | 29  | 8    | 20     |
--+----+-------------------+------------------------+------+-------+-----+------+--------+


-- Check for origin_ariport information
SELECT *
FROM airports
WHERE id =
(
    SELECT origin_airport_id
    FROM flights
    WHERE year = 2025 AND month = 7 AND day = 29
    ORDER BY hour, minute
    LIMIT 1
);
--+----+--------------+-----------------------------+------------+
--| id | abbreviation |          full_name          |    city    |
--+----+--------------+-----------------------------+------------+
--| 8  | CSF          | Fiftyville Regional Airport | Fiftyville |
--+----+--------------+-----------------------------+------------+


-- Checking passengers on the flight 36
SELECT *
FROM passengers
WHERE flight_id =
(
    SELECT id
    FROM flights
    WHERE year = 2025 AND month = 7 AND day = 29
    ORDER BY hour, minute
    LIMIT 1
);


-- Checking passengers names on the flight 36
SELECT name
FROM people
WHERE passport_number IN
(
    SELECT passport_number
    FROM passengers
    WHERE flight_id =
    (
        SELECT id
        FROM flights
        WHERE year = 2025 AND month = 7 AND day = 29
        ORDER BY hour, minute
        LIMIT 1
    )
);
--+--------+
--|  name  |
--+--------+
--| Kenny  |
--| Sofia  |
--| Taylor |
--| Luca   |
--| Kelsey |
--| Edward |
--| Bruce  |
--| Doris  |
--+--------+


-- The THIEF is:
SELECT *
FROM people
WHERE id IN
(
    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN
    (
        SELECT account_number
        FROM atm_transactions
        WHERE year = 2025 AND month = 7 AND day = 28
        AND atm_location = "Leggett Street"
        AND transaction_type = "withdraw"
    )
)
AND license_plate IN
(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2025 AND month = 7 AND day = 28
    AND hour = 10
    AND minute >= 15 AND minute <= 25
    AND activity = "exit"
)
AND phone_number IN
(
    SELECT caller
    FROM phone_calls
    WHERE year = 2025 AND month = 7 AND day = 28
    AND duration < 60
)
AND passport_number IN
(
    SELECT passport_number
    FROM passengers
    WHERE flight_id =
    (
        SELECT id
        FROM flights
        WHERE year = 2025 AND month = 7 AND day = 29
        ORDER BY hour, minute
        LIMIT 1
    )
);
--+--------+-------+----------------+-----------------+---------------+
--|   id   | name  |  phone_number  | passport_number | license_plate |
--+--------+-------+----------------+-----------------+---------------+
--| 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
--+--------+-------+----------------+-----------------+---------------+


-- The city the thief ESCAPED TO:
SELECT city
FROM airports
WHERE id =
(
    SELECT destination_airport_id
    FROM flights
    WHERE year = 2025 AND month = 7 AND day = 29
    ORDER BY hour, minute
    LIMIT 1
);
--+---------------+
--|     city      |
--+---------------+
--| New York City |
--+---------------+


-- The ACCOMPLICE is:
SELECT name
FROM people
WHERE phone_number =
(
    SELECT receiver
    FROM phone_calls
    WHERE year = 2025 AND month = 7 AND day = 28
    AND duration < 60
    AND caller =
    (
        SELECT phone_number
        FROM people
        WHERE name = "Bruce"
    )
);
--+-------+
--| name  |
--+-------+
--| Robin |
--+-------+
