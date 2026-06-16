-- 10. Names of all directors who have directed a movie that got a rating of at least 9.0

-- Find the names of people with those people IDs
SELECt DISTINCT name
FROM people
WHERE id IN
(
    -- Find the IDs of people who directed those movies
    SELECT person_id
    FROM directors
    WHERE movie_id IN
    (
        -- Find the IDs of movies with at least a 9.0 rating
        SELECT movie_id
        FROM ratings
        WHERE rating >= 9
    )
);
