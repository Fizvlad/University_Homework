-------------------------------------------------------------------------------
-- SELECT
-------------------------------------------------------------------------------

-- 1. Get table of all instruments (WHERE, ORDER BY)
SELECT instrument_subtype.name AS 'Подтип', instrument.id AS 'ID', instrument.description AS 'Описание', instrument_type.name AS 'Тип', instrument_class.name AS 'Класс'
	FROM instrument, instrument_subtype, instrument_type, instrument_class
	WHERE instrument.subtype = instrument_subtype.id AND instrument_subtype.type_id = instrument_type.id AND instrument_type.class_id = instrument_class.id
	ORDER BY instrument_subtype.name, instrument.id;

-- 2. Get table of configurations for instrument (WHERE, ORDER BY)
SELECT instrument_subtype.name AS 'Подтип', instrument.id AS 'ID', configuration.date AS 'Дата настройки', configuration.performer AS 'Настройщик'
	FROM instrument, instrument_subtype, configuration
	WHERE instrument.subtype = instrument_subtype.id AND instrument.id = configuration.instrument_id
	ORDER BY configuration.date DESC;

-- 3. Get table of total usage for instruments (SUM, WHERE, ORDER BY)
SELECT instrument_subtype.name AS 'Подтип', instrument.id AS 'ID', SUM(DATEDIFF(DAY, lease.begin_date, lease.end_date)) AS 'Общее время использования (дни)'
	FROM instrument, instrument_subtype, lease
	WHERE instrument.subtype = instrument_subtype.id AND instrument.id = lease.instrument_id
	GROUP BY instrument.id, instrument_subtype.name
	ORDER BY 'Общее время использования (дни)' DESC;

-- 4. Get total income from client
SELECT client.id AS 'ID клиента', SUM(lease.price) AS 'Общая прибыль от клиента'
	FROM client, lease
	WHERE lease.client_id = client.id
	GROUP BY client.id
	ORDER BY SUM(lease.price) DESC;

-- 5. Get table of maximum one-lease-income from instruments (MAX, WHERE, ORDER BY)
SELECT instrument_subtype.name AS 'Подтип', instrument.id AS 'ID', MAX(lease.price) AS 'Наибольшая стоимость заказа'
	FROM instrument, instrument_subtype, lease
	WHERE instrument.subtype = instrument_subtype.id AND instrument.id = lease.instrument_id
	GROUP BY instrument.id, instrument_subtype.name
	ORDER BY 'Наибольшая стоимость заказа' DESC;

-- 6. Get table of all clients who have made any leases (INNER JOIN)
SELECT client_id AS 'ID клиента', instrument_id AS 'ID инструмента', begin_date AS 'Начало аренды', end_date AS 'Конец аренды'
	FROM lease
	INNER JOIN client ON client.id=lease.client_id
	ORDER BY client.id, begin_date, end_date;

-- 7. Get table of all clients who haven't made any leases (OUTER JOIN)
SELECT client.id AS 'ID клиента', client.is_legal AS 'Частное лицо', client.phone AS 'Номер телефона'
	FROM lease
	RIGHT OUTER JOIN client ON client.id=lease.client_id
	WHERE client_id IS NULL;

-- 8. Get table of instruments which were never used (GROUP BY, HAVING)
SELECT instrument_subtype.name AS 'Подтип', instrument.id AS 'ID', instrument.description AS 'Описание'
	FROM instrument
	INNER JOIN instrument_subtype ON instrument.subtype = instrument_subtype.id
	LEFT OUTER JOIN lease ON instrument.id = lease.instrument_id
	GROUP BY instrument.id, instrument_subtype.name, instrument.description
	HAVING COUNT(lease.instrument_id) = 0
	ORDER BY instrument.id ASC;


-- 9 (Alternative to 8). Get table of instruments which were never used (EXISTS)
SELECT instrument_subtype.name AS 'Подтип', instrument.id AS 'ID', instrument.description AS 'Описание'
	FROM instrument, instrument_subtype
	WHERE instrument.subtype = instrument_subtype.id AND NOT EXISTS(
		SELECT * FROM lease WHERE lease.instrument_id = instrument.id
	)
	ORDER BY instrument.id ASC;

-- 10. Get table of all leases for legal clients (IN)
SELECT lease.instrument_id as 'ID инструмента', lease.client_id as 'ID клиента', lease.begin_date AS 'Начало аренды', lease.end_date AS 'Конец аренды'
	FROM lease
	WHERE lease.client_id IN(
		SELECT id FROM client WHERE is_legal = 1
	);


-- 11. Get table of instruments which are always rented for less than 5 days (EXISTS, ALL)
SELECT instrument_subtype.name AS 'Подтип', instrument.id AS 'ID', instrument.description AS 'Описание'
	FROM instrument, instrument_subtype
	WHERE instrument.subtype = instrument_subtype.id 
	AND EXISTS (
		SELECT * FROM lease WHERE lease.instrument_id = instrument.id
	)
	AND 5 >= ALL(
		SELECT DATEDIFF(DAY, lease.begin_date, lease.end_date) FROM lease WHERE lease.instrument_id = instrument.id
	);

-- 12. Get table of identification numbers for all clients
(SELECT registration_number AS 'Регитрационный номер / Номер пасспорта' FROM legal_client_data) UNION (SELECT passport_number FROM private_client_data)

-------------------------------------------------------------------------------
-- CREATE VIEW
-------------------------------------------------------------------------------
GO
CREATE VIEW client_lease_view AS
	SELECT *
		FROM lease
		RIGHT OUTER JOIN client ON client.id=lease.client_id;
GO

-------------------------------------------------------------------------------
-- USE VIEW
-------------------------------------------------------------------------------

SELECT client_lease_view.phone AS 'Номер телефона', instrument_subtype.name AS 'Инструмент', client_lease_view.begin_date AS 'Начало аренды', client_lease_view.end_date AS 'Конец аренды'
	FROM client_lease_view, instrument_subtype
	WHERE instrument_subtype.id = client_lease_view.instrument_id
	ORDER BY client_lease_view.end_date;
