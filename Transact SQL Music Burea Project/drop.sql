-------------------------------------------------------------------------------
-- DROP VIEW
-------------------------------------------------------------------------------

DROP VIEW client_lease_view;

-------------------------------------------------------------------------------
-- DROP ALL TRIGGERS
-------------------------------------------------------------------------------

DROP TRIGGER lease_count_price;
DROP FUNCTION get_price; -- Droping utility function for trigger

-------------------------------------------------------------------------------
-- DROP ALL INDEXES
-------------------------------------------------------------------------------

DROP INDEX client_phone_indx ON client;
DROP INDEX lease_client_instrument_indx ON lease;


-------------------------------------------------------------------------------
-- DROP ALL TABLES
-------------------------------------------------------------------------------

ALTER TABLE instrument DROP CONSTRAINT instrument_fk0;
ALTER TABLE instrument_subtype DROP CONSTRAINT instrument_subtype_fk0;
ALTER TABLE instrument_type DROP CONSTRAINT instrument_type_fk0;
ALTER TABLE configuration DROP CONSTRAINT configuration_fk0;
ALTER TABLE legal_client_data DROP CONSTRAINT legal_client_data_fk0;
ALTER TABLE private_client_data DROP CONSTRAINT private_client_data_fk0;
ALTER TABLE lease DROP CONSTRAINT lease_fk0;
ALTER TABLE lease DROP CONSTRAINT lease_fk1;
ALTER TABLE price DROP CONSTRAINT price_fk0;
ALTER TABLE price DROP CONSTRAINT price_uq;

DROP TABLE client;
DROP TABLE instrument;
DROP TABLE instrument_subtype;
DROP TABLE instrument_type;
DROP TABLE instrument_class;
DROP TABLE configuration;
DROP TABLE legal_client_data;
DROP TABLE private_client_data;
DROP TABLE lease;
DROP TABLE price;


-------------------------------------------------------------------------------
-- DROP ALL FUNCTIONS AND PROCEDURES
-------------------------------------------------------------------------------

DROP FUNCTION if_intersect;

DROP PROCEDURE add_legal_client;
DROP PROCEDURE add_private_client;
DROP PROCEDURE add_instrument_class;
DROP PROCEDURE add_instrument_type;
DROP PROCEDURE add_instrument_subtype;
DROP PROCEDURE add_instrument;
DROP PROCEDURE add_configuration;
DROP PROCEDURE add_price;
DROP PROCEDURE add_lease;

DROP PROCEDURE remove_instrument;

DROP PROCEDURE inspect_instrument;
DROP PROCEDURE rate_lease;

DROP FUNCTION get_rating;