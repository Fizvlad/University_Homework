# Tickets from station

SELECT DISTINCT
    t.TICKET_ID,
    p.PASSENGER_ID,
    v.VOYAGE_ID,
    r.ROUTE_ID,
    tr.TO_ID
FROM
    tickets t,
    passengers p,
    ticket_voyage tv,
    voyages v,
    routes r,
    trails tr
WHERE
    t.PASSENGER_ID = p.PASSENGER_ID
        AND t.TICKET_ID = tv.TICKET_ID
        AND tv.VOYAGE_ID = v.VOYAGE_ID
        AND v.ROUTE_ID = r.ROUTE_ID
        AND r.TRAIL_ID = tr.TRAIL_ID
        
        AND tr.FROM_ID = 1
ORDER BY t.TICKET_ID;