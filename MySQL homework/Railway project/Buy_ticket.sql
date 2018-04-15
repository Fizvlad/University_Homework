# "Buy a ticket". Actually just all the voyages from A to B

SET @u_from_station_id = 1;
SET @u_to_station_id = 2;

SELECT DISTINCT
    v.VOYAGE_ID,
    vi.DATE,
    vi.TIME,
    vi.TRAIN_ID
FROM
    voyages v,
    voyage_info vi,
    routes r,
    trails t
WHERE
    v.VOYAGE_ID = vi.VOYAGE_ID
        AND v.ROUTE_ID = r.ROUTE_ID
        AND r.TRAIL_ID = t.TRAIL_ID
        AND t.FROM_ID = @u_from_station_id
        AND t.TO_ID = @u_to_station_id
ORDER BY vi.DATE ASC , vi.TIME ASC;