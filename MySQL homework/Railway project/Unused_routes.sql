# Get routes which weren't used for more than N days

SET @u_N = 2;

SELECT 
    ROUTE_ID, NOT_USED_FOR
FROM
    (SELECT 
        r.ROUTE_ID, MIN(CURRENT_DATE() - vi.DATE) AS NOT_USED_FOR
    FROM
        routes r, voyages v, voyage_info vi
    WHERE
        v.VOYAGE_ID = vi.VOYAGE_ID
            AND r.ROUTE_ID = v.ROUTE_ID
    GROUP BY r.ROUTE_ID
    ORDER BY r.ROUTE_ID) AS NOT_USED_FOR_TABLE
WHERE
    NOT_USED_FOR >= @u_N;