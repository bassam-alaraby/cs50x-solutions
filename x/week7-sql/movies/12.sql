-- 12. Titles of all of movies in which both Jennifer Lawrence and Bradley Cooper starred

-- Find movie titles from the movie IDs associated with both Bradley Cooper and Jennifer Lawrence
SELECT title
FROM movies
WHERE id IN
(
    -- Find the IDs of movies associated with Bradley Cooper’s ID
    SELECT movie_id
    FROM stars
    WHERE person_id =
    (
        -- Find the ID of Bradley Cooper
        SELECT id
        FROM people
        WHERE name = 'Bradley Cooper'
    )
)
AND id IN
(
    -- Find the IDs of movies associated with Jennifer Lawrence’s ID
    SELECT movie_id
    FROM stars
    WHERE person_id =
    (
        -- Find the ID of Jennifer Lawrence
        SELECT id
        FROM people
        WHERE name = 'Jennifer Lawrence'
    )
);
