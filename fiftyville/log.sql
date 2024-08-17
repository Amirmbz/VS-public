-- Keep a log of any SQL queries you execute as you solve the mystery.


-- Lets see what we have.
.table


-- We have the crime_scene_reports table. Lets look into it.
.schema crime_scene_reports


-- Frist I found the report of the theft. I figured that the report description should containe CS50 in it
-- We can find the report by intering the place and and time but it count down to two theft so it's not optimal to do that.
SELECT * FROM crime_scene_repots WHERE description LIKE '%CS50%';


-- The discription mentioned the theft took place at the bakery
-- we have a bakery table as well. So lets see what's in the bakery table.
SELECT * FROM bakery_security_logs
    WHERE year = 2023
      AND month = 7
      AND day = 28;


-- In the bakery security log we can find out what license plates intered and exited at what time
-- So we want to find out at the time of the theft what license plates were in the bakery
SELECT * FROM bakery_security_logs
    WHERE year = 2023
      AND month = 7
      AND day = 28
      AND hour < 10
      AND activity = 'entrance'
      OR (hour = 10
      AND minute <= 15
      AND year = 2023
      AND month = 7
      AND day = 28
      AND activity = 'entrance') -- This is every entrance that have happened before the theft
    EXCEPT -- now substarct the people who left before the theft
SELECT * FROM bakery_security_logs
    WHERE activity = 'exit'
      AND year = 2023
      AND month = 7
      AND day = 28
      AND hour < 10;


-- So we can convert these license plates to persons names
SELECT * FROM people WHERE license_plate IN
(SELECT license_plate FROM bakery_security_logs
    WHERE year = 2023
      AND month = 7
      AND day = 28
      AND hour < 10
      AND activity = 'entrance'
      OR (hour = 10
      AND minute <= 15
      AND year = 2023
      AND month = 7
      AND day = 28
      AND activity = 'entrance')
    EXCEPT
SELECT license_plate FROM bakery_security_logs
    WHERE activity = 'exit'
      AND year = 2023
      AND month = 7
      AND day = 28
      AND hour < 10);


-- So that's a long list by taking another look at our tables we find the interviews table that might be helpful
.schema interviews


-- Lets look at the transcripts for our interviews
SELECT * FROM interviews
  WHERE day = 28
    AND month = 7
    AND year = 2023;

-- The interwes mentioned that the thief left the bakery within 10 minutes time frame lets check into that
SELECT * FROM people WHERE license_plate IN
(SELECT license_plate FROM bakery_security_logs
    WHERE year = 2023
      AND month = 7
      AND day = 28
      AND hour = 10
      AND minute < 25
      AND activity = 'exit');


-- Lets take a look at the transactions at that time in the leggett street
SELECT * FROM atm_transactions
  WHERE day = 28
    AND month = 7
    AND year = 2023
    AND atm_location = 'Leggett Street';


-- Lets see who are these bank acounts for
SELECT * FROM people WHERE id IN
(SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions
  WHERE day = 28
  AND month = 7
  AND year = 2023
  AND atm_location = 'Leggett Street'));


-- The interviews also mentioned that the thief had phone call so lets look into it
SELECT * FROM phone_calls
  WHERE year = 2023
    AND month = 7
    AND day = 28
    AND duration <= 60;


-- So lets find some names
SELECT * FROM people WHERE phone_number IN
(SELECT caller FROM phone_calls
  WHERE year = 2023
  AND month = 7
  AND day = 28
  AND duration <= 60);

-- One of the other interviews mentioned that they were gonna fly out of fiftyville
-- So lets take a look at the airports table and where that take us
SELECT * FROM airports WHERE city = 'Fiftyville';


-- Now lets check the earliest flights that leaving this airport the day after the theft
SELECT * FROM flights
  WHERE day = 29
    AND month = 7
    AND year = 2023
    AND origin_airport_id =
      (SELECT id FROM airports WHERE city = 'Fiftyville')
    ORDER BY hour,minute LIMIT 1;


-- So lets see where that flight go to
SELECT city FROM airports WHERE id IN                                  --*******city the thief ESCAPED TO*******
  (SELECT destination_airport_id FROM flights
    WHERE day = 29
      AND month = 7
      AND year = 2023
      AND origin_airport_id =
        (SELECT id FROM airports WHERE city = 'Fiftyville')
      ORDER BY hour,minute LIMIT 1);


-- Now lets list all the passengers of that flight
SELECT * FROM people WHERE passport_number IN
  (SELECT passport_number FROM passengers WHERE flight_id IN
    (SELECT id FROM flights
      WHERE day = 29
        AND month = 7
        AND year = 2023
        AND origin_airport_id =
          (SELECT id FROM airports WHERE city = 'Fiftyville')
        ORDER BY hour,minute LIMIT 1));


-- Now lets see how many people where in all these lists of people
SELECT * FROM people WHERE license_plate IN                                  --*******THIEF*******
  (SELECT license_plate FROM bakery_security_logs
    WHERE year = 2023
      AND month = 7
      AND day = 28
      AND hour = 10
      AND minute < 25
      AND activity = 'exit')
  AND id IN
    (SELECT person_id FROM bank_accounts WHERE account_number IN
      (SELECT account_number FROM atm_transactions
        WHERE day = 28
          AND month = 7
          AND year = 2023
          AND atm_location = 'Leggett Street'))
  AND phone_number IN
    (SELECT caller FROM phone_calls
      WHERE year = 2023
        AND month = 7
        AND day = 28
        AND duration <= 60)
  AND passport_number IN
    (SELECT passport_number FROM passengers WHERE flight_id IN
      (SELECT id FROM flights
        WHERE day = 29
          AND month = 7
          AND year = 2023
          AND origin_airport_id =
            (SELECT id FROM airports WHERE city = 'Fiftyville')
        ORDER BY hour,minute LIMIT 1));


-- Well how about that, there is only one person how were on all of those lists
-- Now lest see who he called
SELECT * FROM people WHERE phone_number IN                                  --*******ACCOMPLICE*******
  (SELECT receiver FROM phone_calls WHERE caller IN
    (SELECT phone_number FROM people WHERE license_plate IN
      (SELECT license_plate FROM bakery_security_logs
        WHERE year = 2023
          AND month = 7
          AND day = 28
          AND hour = 10
          AND minute < 25
          AND activity = 'exit')
      AND id IN
        (SELECT person_id FROM bank_accounts WHERE account_number IN
          (SELECT account_number FROM atm_transactions
            WHERE day = 28
              AND month = 7
              AND year = 2023
              AND atm_location = 'Leggett Street'))
      AND phone_number IN
        (SELECT caller FROM phone_calls
          WHERE year = 2023
            AND month = 7
            AND day = 28
            AND duration <= 60)
      AND passport_number IN
        (SELECT passport_number FROM passengers WHERE flight_id IN
          (SELECT id FROM flights
            WHERE day = 29
              AND month = 7
              AND year = 2023
              AND origin_airport_id =
                (SELECT id FROM airports WHERE city = 'Fiftyville')
            ORDER BY hour,minute LIMIT 1)))
    AND year = 2023
    AND month = 7
    AND day = 28
    AND duration <= 60);






