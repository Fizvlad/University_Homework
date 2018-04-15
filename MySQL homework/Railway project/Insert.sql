# Stations
insert into stations values(1);
insert into stations values(2);
insert into stations values(3);

# Trains
insert into trains values(1);
insert into trains values(2);
insert into trains values(3);

# Passengers
insert into passengers values(1);
insert into passengers values(2);
insert into passengers values(3);
insert into passengers values(4);
insert into passengers values(5);
insert into passengers values(6);


# Trails
insert into trails values(1, 1, 2);
insert into trails values(2, 2, 3);
insert into trails values(3, 3, 1);

# Routes
insert into routes values(1, 1);
insert into routes values(2, 3);
insert into routes values(3, 3);
insert into routes values(4, 2);
insert into routes values(5, 1);

# Voyages
insert into voyages values(1, 1);
insert into voyages values(2, 2);
insert into voyages values(3, 3);
insert into voyages values(4, 4);
insert into voyages values(5, 5);
insert into voyages values(6, 1);
insert into voyages values(7, 2);

# Voyage info
insert into voyage_info values(1, '2018-4-12', '10:00:00', 1);
insert into voyage_info values(2, '2018-4-12', '16:30:00', 2);
insert into voyage_info values(3, '2018-4-12', '20:18:00', 3);
insert into voyage_info values(4, '2018-4-13', '10:00:00', 1);
insert into voyage_info values(5, '2018-4-13', '16:00:00', 2);
insert into voyage_info values(6, '2018-4-13', '20:00:00', 3);
insert into voyage_info values(7, '2018-4-14', '6:00:00', 1);

# Tickets
insert into tickets values(1, 1);
insert into tickets values(2, 1);
insert into tickets values(3, 2);
insert into tickets values(4, 2);
insert into tickets values(5, 3);
insert into tickets values(6, 3);
insert into tickets values(7, 4);
insert into tickets values(8, 4);
insert into tickets values(9, 5);
insert into tickets values(10, 5);
insert into tickets values(11, 6);
insert into tickets values(12, 6);

# Tickets info
insert into ticket_voyage values(1, 1);
insert into ticket_voyage values(2, 2);
insert into ticket_voyage values(3, 3);
insert into ticket_voyage values(4, 4);
insert into ticket_voyage values(5, 5);
insert into ticket_voyage values(6, 6);
insert into ticket_voyage values(7, 7);
insert into ticket_voyage values(8, 1);
insert into ticket_voyage values(9, 2);
insert into ticket_voyage values(10, 3);
insert into ticket_voyage values(11, 4);
insert into ticket_voyage values(12, 5);
insert into ticket_voyage values(1, 2);
insert into ticket_voyage values(1, 3);
insert into ticket_voyage values(7, 1);
insert into ticket_voyage values(8, 1);
insert into ticket_voyage values(10, 1);