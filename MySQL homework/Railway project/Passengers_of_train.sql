# All passengers of train on day

SET @u_train_id := 1;
SET @u_date := '2018-04-12';

SELECT DISTINCT
    p.PASSENGER_ID,
    t.TICKET_ID,
    vi.VOYAGE_ID,
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
        AND tr.TRAIN_ID = @u_train_id
        AND vi.DATE = @u_date
ORDER BY p.PASSENGER_ID;
