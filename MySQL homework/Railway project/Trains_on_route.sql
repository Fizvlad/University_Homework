# Trains on route

SET @u_route_id := 1;

SELECT DISTINCT
    t.TRAIN_ID, vi.VOYAGE_ID, vi.DATE, vi.TIME
FROM
    trains t,
    voyage_info vi,
    voyages v
WHERE
    t.TRAIN_ID = vi.TRAIN_ID
        AND vi.VOYAGE_ID = v.VOYAGE_ID
        AND v.ROUTE_ID = @u_route_id
ORDER BY t.TRAIN_ID;
    