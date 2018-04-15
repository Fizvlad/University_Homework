# All passengers of train on day

SELECT DISTINCT
    p.PASSENGER_ID,
    t.TICKET_ID,
    vi.VOYAGE_ID,
    tr.TRAIN_ID,
    vi.DATE,
    vi.TIME
FROM
    passengers p,
    tickets t,
    voyage_info vi,
    trains tr,
    ticket_voyage tv
WHERE
    p.PASSENGER_ID = t.PASSENGER_ID
        AND t.TICKET_ID = tv.TICKET_ID
        AND tv.VOYAGE_ID = vi.VOYAGE_ID
        AND vi.TRAIN_ID = tr.TRAIN_ID
        
        AND tr.TRAIN_ID = 1
        AND vi.DATE = '2018-04-12'
ORDER BY p.PASSENGER_ID;
