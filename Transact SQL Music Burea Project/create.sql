-------------------------------------------------------------------------------
-- CREATE DATABASE TABLES
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
-- CLIENTS
-------------------------------------------------------------------------------

CREATE TABLE client (
	id INTEGER IDENTITY(0, 1), -- PK
	is_legal BIT NOT NULL, -- 0 / 1
	phone CHAR(10) NOT NULL UNIQUE CHECK (phone NOT LIKE '%[^0-9]%'), -- Checked on data addition
	address VARCHAR(200) NOT NULL, -- Need no check
	
	CONSTRAINT client_pk PRIMARY KEY (id)
);



CREATE TABLE legal_client_data (
	client_id INTEGER NOT NULL, -- FK
	registration_number CHAR(13) NOT NULL UNIQUE CHECK (registration_number NOT LIKE '%[^0-9]%'), -- Simple check
	
	CONSTRAINT legal_client_data_pk PRIMARY KEY (client_id)
);



CREATE TABLE private_client_data (
	client_id INTEGER NOT NULL, -- FK
	passport_number CHAR(10) NOT NULL UNIQUE CHECK (passport_number NOT LIKE '%[^0-9]%'), -- Checked on data addition
	
	CONSTRAINT private_client_data_pk PRIMARY KEY (client_id)
);

-------------------------------------------------------------------------------
-- INSTRUMENTS
-------------------------------------------------------------------------------

CREATE TABLE instrument (
	id INTEGER, -- PK (Manually updated)
	description VARCHAR(150), -- Need no check
	subtype INTEGER NOT NULL, -- FK
	last_inspection DATE NOT NULL DEFAULT (CONVERT (DATE, CURRENT_TIMESTAMP)), -- Current date by default
	
	CONSTRAINT instrument_pk PRIMARY KEY (id)
);



CREATE TABLE instrument_subtype (
	id INTEGER IDENTITY(0, 1), -- PK
	name VARCHAR(50) NOT NULL UNIQUE, -- Need no check
	type_id INTEGER NOT NULL, -- FK
	
	CONSTRAINT instrument_subtype_pk PRIMARY KEY (id)
);



CREATE TABLE instrument_type (
	id INTEGER IDENTITY(0, 1), -- PK
	name VARCHAR(50) NOT NULL UNIQUE, -- Need no check
	class_id INTEGER NOT NULL, -- FK
	
	CONSTRAINT instrument_type_pk PRIMARY KEY (id)
);



CREATE TABLE instrument_class (
	id INTEGER IDENTITY(0, 1), -- PK
	name VARCHAR(50) NOT NULL UNIQUE, -- Need no check
	
	CONSTRAINT instrument_class_pk PRIMARY KEY (id)
);



CREATE TABLE configuration (
	instrument_id INTEGER NOT NULL, -- FK
	date DATE NOT NULL, -- Not empty
	performer VARCHAR(50) NOT NULL CHECK (performer <> '') -- Not empty string
);



CREATE TABLE price (
	instrument_id INTEGER NOT NULL, -- FK
	lease_min_length INTEGER NOT NULL CHECK (lease_min_length > 0), -- Minimum amount of days for price to work
	is_legal BIT NOT NULL, -- 0 / 1
	per_day INTEGER NOT NULL CHECK (per_day > 0), -- price per day > 0

	CONSTRAINT price_uq UNIQUE(instrument_id, lease_min_length, is_legal) -- This combination must be unique
);

-------------------------------------------------------------------------------
-- RENT
-------------------------------------------------------------------------------

CREATE TABLE lease (
	client_id INTEGER NOT NULL, -- FK
	begin_date DATE NOT NULL, -- Current date by default
	end_date DATE NOT NULL, -- later than begin date (via CONSTRAINT)
	rate INTEGER CHECK (rate >= 1 AND rate <= 10), -- integer between from 1 to 10
	instrument_id INTEGER NOT NULL, -- FK
	price INTEGER, -- Calculated automatically with trigger
	
	CONSTRAINT lease_datacheck CHECK (end_date > begin_date) -- Checking data
);


-------------------------------------------------------------------------------
-- FOREIGN KEYS
-------------------------------------------------------------------------------

ALTER TABLE instrument ADD CONSTRAINT instrument_fk0 FOREIGN KEY (subtype) REFERENCES instrument_subtype(id);

ALTER TABLE instrument_subtype ADD CONSTRAINT instrument_subtype_fk0 FOREIGN KEY (type_id) REFERENCES instrument_type(id);

ALTER TABLE instrument_type ADD CONSTRAINT instrument_type_fk0 FOREIGN KEY (class_id) REFERENCES instrument_class(id);

ALTER TABLE configuration ADD CONSTRAINT configuration_fk0 FOREIGN KEY (instrument_id) REFERENCES instrument(id);

ALTER TABLE price ADD CONSTRAINT price_fk0 FOREIGN KEY (instrument_id) REFERENCES instrument(id);

ALTER TABLE legal_client_data ADD CONSTRAINT legal_client_data_fk0 FOREIGN KEY (client_id) REFERENCES client(id);

ALTER TABLE private_client_data ADD CONSTRAINT private_client_data_fk0 FOREIGN KEY (client_id) REFERENCES client(id);

ALTER TABLE lease ADD CONSTRAINT lease_fk0 FOREIGN KEY (client_id) REFERENCES client(id);
ALTER TABLE lease ADD CONSTRAINT lease_fk1 FOREIGN KEY (instrument_id) REFERENCES instrument(id);


-------------------------------------------------------------------------------
-- CREATE INDEX
-------------------------------------------------------------------------------

CREATE UNIQUE INDEX client_phone_indx ON client(phone);

CREATE NONCLUSTERED INDEX lease_client_instrument_indx ON lease(client_id, instrument_id);


-------------------------------------------------------------------------------
-- CREATE TRIGGER
-------------------------------------------------------------------------------

-- Utility function: Get price per day for given instrument on given dates
GO
CREATE FUNCTION get_price  
(
	@instrument_id INTEGER,
	@period INTEGER,
	@client_id INTEGER
)  
RETURNS INTEGER
AS BEGIN
	DECLARE @is_legal BIT;
	SET @is_legal = (SELECT is_legal FROM client WHERE id = @client_id);
	DECLARE @per_day INTEGER;
	SET @per_day = (SELECT TOP 1 per_day FROM price WHERE instrument_id = @instrument_id AND is_legal = @is_legal AND lease_min_length <= @period ORDER BY lease_min_length DESC);
	RETURN @per_day * @period;
END;
GO

CREATE TRIGGER lease_count_price ON lease AFTER INSERT
AS BEGIN	
	UPDATE lease SET price = dbo.get_price(instrument_id, DATEDIFF(DAY, begin_date, end_date), client_id) WHERE price IS NULL;
END;
GO

