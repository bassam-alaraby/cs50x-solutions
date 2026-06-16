-- 11. Titles of the five highest rated movies (in order) that Chadwick Boseman starred in, starting with the highest rated

-- Find the movie titles with those movie IDs
SELECT title
FROM movies
JOIN ratings ON movies.id = ratings.movie_id
WHERE id IN
(
    -- Find the IDs of movies associated with Chadwick Boseman’s ID
    SELECT movie_id
    FROM stars
    WHERE person_id =
    (
        -- Find the ID of Chadwick Boseman
        SELECT id
        FROM people
        WHERE name = "Chadwick Boseman"
    )
)
ORDER BY rating DESC
LIMIT 5;
