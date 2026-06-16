-- 9. Names and IDs of all people who starred in a movie released in 2004, ordered by birth year

-- Find the IDs of people who starred in those movies
SELECT DISTINCT name, id
FROM people
WHERE id IN
(
    -- Find the IDs of people who starred in those movies
    SELECT person_id
    FROM stars
    WHERE movie_id IN
    (
        -- Find the IDs of movies released in 2004
        SELECT id
        FROM movies
        WHERE year = 2004
    )
)
ORDER BY birth;
