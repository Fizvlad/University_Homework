#!/usr/bin/ruby -w

# mysql_interaction.rb - Simple examples of request to MySQL from Ruby
# Guide on MySQL in Ruby: http://www.kitebird.com/articles/ruby-mysql.html

require "mysql"

Host = "localhost"
User = "testuser"
Password = "testpassword"
DB_Name = "test"
Table_Name = "users"

begin
    dbh = Mysql.real_connect(Host, User, Password, DB_Name) # Connecting
    puts "      INFO : Server version: " + dbh.get_server_info
    puts ""

    # Checking if table already exists
    matchingTables = dbh.query("SHOW TABLES LIKE '#{Table_Name}'")
    ifFoundTable = matchingTables.num_rows != 0
    matchingTables.free
    if ifFoundTable
        puts "  Found table '#{Table_Name}'"
        puts "  Do you want to delete it? Type Y to delete it, type anything else otherwise"
        if gets.chomp == "Y"
            dbh.query("DROP TABLE #{Table_Name}")
            puts "  Deleted table"
            abort
        end
    else
        puts "  Table '#{Table_Name}' was not found. Creating one"
        dbh.query("
            CREATE TABLE #{Table_Name} (
                id INT UNSIGNED,
                name CHAR(64) CHARACTER SET utf8,
                dob DATE
            )
            ")
        dbh.query("ALTER TABLE users ADD UNIQUE (id)")

        dbh.query("INSERT INTO #{Table_Name} (id, name, dob)
                    VALUES
                      (1, 'Pasha', '2000-01-01'),
                      (615616, 'Vanya', '2000-02-04'),
                      (11111, 'Masha', '2000-03-09'),
                      (1337, 'Dasha', '2000-04-16')
                  ")
    end

    response = dbh.query("SELECT * FROM users")
    puts "  Table:"
    response.each do |row|
        puts "    #{row[0]}, #{row[1]}, #{row[2]}"
    end
    puts "  Number of rows returned: #{response.num_rows}"

    response.free
rescue Mysql::Error => e
    puts "      Error code: #{e.errno}"
    puts "      Error message: #{e.error}"
    puts "      Error SQLSTATE: #{e.sqlstate}" if e.respond_to?("sqlstate")
ensure
    dbh.close if dbh
end
