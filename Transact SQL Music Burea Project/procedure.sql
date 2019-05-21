-------------------------------------------------------------------------------
-- UTILITY FUNCTIONS
-------------------------------------------------------------------------------

-- Check whether given instrument will be able on given dates
CREATE FUNCTION if_intersect  
(
	@instrument_id INTEGER,
	@begin_date DATE,
	@end_date DATE
)  
RETURNS BIT
AS BEGIN
	IF @end_date < @begin_date RETURN 1;

	DECLARE @intersection_end_date DATE;
	SET @intersection_end_date = (SELECT TOP 1 end_date FROM lease WHERE instrument_id = @instrument_id AND begin_date <= @end_date AND end_date >= @begin_date);
	IF @intersection_end_date IS NULL RETURN 0;
	RETURN 1;
END;
GO

-------------------------------------------------------------------------------
-- INSERTION PROCEDURES
-------------------------------------------------------------------------------

-- Add legal client
CREATE PROCEDURE add_legal_client
	@phone CHAR(11),
	@address VARCHAR(200),
	@registration_number CHAR(13)
AS BEGIN
	INSERT INTO client(is_legal, phone, address) VALUES(1, @phone, @address);
	DECLARE @id INTEGER;
	SET @id = SCOPE_IDENTITY();
	INSERT INTO legal_client_data(client_id, registration_number) VALUES(@id, @registration_number);
END;
GO -- Have to split this sql into several batches

-- Add private client
CREATE PROCEDURE add_private_client
	@phone CHAR(11),
	@address VARCHAR(200),
	@passport_number CHAR(10)
AS BEGIN
	INSERT INTO client(is_legal, phone, address) VALUES(0, @phone, @address);
	DECLARE @id INTEGER;
	SET @id = SCOPE_IDENTITY();
	INSERT INTO private_client_data(client_id, passport_number) VALUES(@id, @passport_number);
END;
GO


-- Add class for instrument
CREATE PROCEDURE add_instrument_class
	@name VARCHAR(50)
AS BEGIN
	INSERT INTO instrument_class(name) VALUES(@name);
END;
GO

-- Add type for instrument
CREATE PROCEDURE add_instrument_type
	@name VARCHAR(50),
	@class_name VARCHAR(50)
AS BEGIN
	DECLARE @class_id INTEGER;
	SET @class_id = (SELECT id FROM instrument_class WHERE name = @class_name);
	IF @class_id IS NULL BEGIN
		RAISERROR ('Unknown instrument class: %s', 16, 1, @class_name);
		RETURN 1;
	END;
	INSERT INTO instrument_type(name, class_id) VALUES(@name, @class_id);
END;
GO

-- Add subtype for instrument
CREATE PROCEDURE add_instrument_subtype
	@name VARCHAR(50),
	@type_name VARCHAR(50)
AS BEGIN
	DECLARE @type_id INTEGER;
	SET @type_id = (SELECT id FROM instrument_type WHERE name = @type_name);
	IF @type_id IS NULL BEGIN
		RAISERROR ('Unknown instrument type: %s', 16, 1, @type_name);
		RETURN 1;
	END;
	INSERT INTO instrument_subtype(name, type_id) VALUES(@name, @type_id);		
END;
GO

-- Add instrument (Manual indexing)
CREATE PROCEDURE add_instrument
	@subtype_name VARCHAR(50),
	@description VARCHAR(150) = NULL
AS BEGIN
	DECLARE @id INTEGER;
	SET @id = (SELECT MAX(id) + 1 FROM instrument);
	IF @id IS NULL SET @id = 0;

	DECLARE @subtype_id INTEGER;
	SET @subtype_id = (SELECT id FROM instrument_subtype WHERE name = @subtype_name);
	IF @subtype_id IS NULL BEGIN
		RAISERROR ('Unknown instrument subtype: %s', 16, 1, @subtype_name);
		RETURN 1;
	END;
	INSERT INTO instrument(id, description, subtype) VALUES(@id, @description, @subtype_id);
END;
GO

-- Add instrument configuration
CREATE PROCEDURE add_configuration
	@instrument_id INTEGER,
	@performer VARCHAR(50),
	@date DATE = NULL
AS BEGIN
	IF @date IS NULL SET @date = CONVERT (DATE, CURRENT_TIMESTAMP);
	INSERT INTO configuration(instrument_id, date, performer) VALUES(@instrument_id, @date, @performer);
END;
GO

-- Add price category
CREATE PROCEDURE add_price
	@instrument_id INTEGER,
	@lease_min_length INTEGER,
	@is_legal BIT,
	@per_day INTEGER
AS BEGIN
	INSERT INTO price(instrument_id, lease_min_length, is_legal, per_day) VALUES(@instrument_id, @lease_min_length, @is_legal, @per_day);
END;
GO

-- Add lease
CREATE PROCEDURE add_lease
	@client_id INTEGER,
	@instrument_id INTEGER,
	@begin_date DATE,
	@end_date DATE
AS BEGIN
	IF @end_date < @begin_date BEGIN
		RAISERROR ('Begin date must be less than end date', 16, 1);
		RETURN 1;
	END;
	DECLARE @intersect BIT;
	EXEC @intersect = if_intersect @instrument_id, @begin_date, @end_date;
	IF @intersect = 1 BEGIN
		RAISERROR ('This dates are already reserved', 16, 1);
		RETURN 1;
	END;	
	INSERT INTO lease(client_id, begin_date, end_date, instrument_id) VALUES(@client_id, @begin_date, @end_date, @instrument_id);
END;
GO


-------------------------------------------------------------------------------
-- REMOVING PROCEDURES
-------------------------------------------------------------------------------

-- Remove instrument (DELETE)
CREATE PROCEDURE remove_instrument
	@id INTEGER
AS BEGIN
	DELETE FROM lease WHERE lease.instrument_id = @id;
	DELETE FROM price WHERE price.instrument_id = @id;
	DELETE FROM configuration WHERE configuration.instrument_id = @id;
	DELETE FROM instrument WHERE instrument.id = @id;
END;
GO

-------------------------------------------------------------------------------
-- OTHER PROCEDURES AND FUNCTIONS
-------------------------------------------------------------------------------

-- Inspect instrument (UPDATE)
CREATE PROCEDURE inspect_instrument
	@instrument_id INTEGER,
	@date DATE = NULL
AS BEGIN
	IF @date = NULL SET @date = CONVERT(DATE, CURRENT_TIMESTAMP);
	UPDATE instrument SET last_inspection = @date WHERE id = @instrument_id;
END;
GO

-- Set rating for lease
CREATE PROCEDURE rate_lease
	@client_id INTEGER,
	@date DATE,
	@rate INTEGER
AS BEGIN
	UPDATE lease SET rate = @rate WHERE client_id = @client_id AND @date >= begin_date AND @date <= end_date;
END;
GO

-- Get client rating
CREATE FUNCTION get_rating  
(
	@client_id INTEGER
)  
RETURNS FLOAT
AS BEGIN	
	DECLARE @rating FLOAT;
	SET @rating = (SELECT AVG(CAST(rate AS FLOAT)) FROM lease WHERE client_id = @client_id);
	RETURN @rating;
END;
GO