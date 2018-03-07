#Returns random date (almost lol) string
def random_date(min_year = 1980, max_year = 2000)
    return (rand(28) + 1).to_s  + "." + (rand(12) + 1).to_s + "." + (rand(max_year + 1 - min_year) + min_year).to_s
end

# Return new array same to given one but with .to_s applied to each value
def to_s_array(arr)
    result = []
    for value in arr
        if value.is_a?(Array)
            result.push(to_s_array(value))
        else
            result.push(value.to_s)
        end
    end
    return result
end


class Table
    private
    @@splitter = "|"

    @name = ""
    @values = []

    public
    # Saving name of file, names of values
    # Creates file "#{name}.rdb" and adding names of values in first line
    def initialize(name, values)
        # Pre
        name = name.to_s
        values = to_s_array(values)

        # Check
        if !values.is_a?(Array)
            raise ArgumentError, "values must be of array type"
        end
        if values.uniq.length != values.length
            raise ArgumentError, "Some values are not unique: #{values}"
        end

        # Main
        @name = name + ".rdb"
        @values = values

        File.open(@name, "w+") { |file|
            for value in values
                file.write(value + @@splitter)
            end
            file.write("\n")
        }
    end


    # Adding line to file
    def pushLine(values)
        # Pre
        values = to_s_array(values)

        # Check
        if !values.is_a?(Array)
            raise ArgumentError, "values must be of array type"
        end
        if @values.length != values.length
            raise ArgumentError, "Given values (#{values}) doesn't match required values(#{@values})"
        end

        # Main
        File.open(@name, "a+") { |file|
            for value in values
                file.write(value + @@splitter)
            end
            file.write("\n")
        }
    end


    # Adding line if it doesn't exist already
    def pushLineIfUniq(values)
        if lineExists?(values)
            return false
        else
            pushLine(values)
            return true
        end
    end


    # Returnes true if stated line exists
    def lineExists?(values)
        # Pre
        values = to_s_array(values)

        # Check
        if !values.is_a?(Array)
            raise ArgumentError, "values must be of array type"
        end
        if @values.length != values.length
            raise ArgumentError, "Given values (#{values}) doesn't match required values(#{@values})"
        end

        # Main
        strForSearch = ""
        for value in values
            strForSearch += value + @@splitter
        end

        File.open(@name, "r") { |file|
            for line in file
                if line == strForSearch
                    return true
                end
            end
        }
        return false
    end


    # Returnes array of lines(line is array of values) which match given request
    # Request is array: [["valueNameI", "valueI"], ["valueNameJ", "valueJ"], ...]
    def find(requestArray)
        # Pre
        requestArray = to_s_array(requestArray)

        # Check
        if !requestArray.is_a?(Array)
            raise ArgumentError, "requestArray must be of array type"
        end
        for request in requestArray
            if !request.is_a?(Array) || request.length != 2
                raise ArgumentError, "Request must have following type: [\"valueNameI\", \"valueI\"]"
            end
        end

        # Main
        result = []
        File.open(@name, "r") { |file|
            for line in file
                values = line.split(@@splitter)
                values.delete_at(values.length - 1)

                isMatching = true
                for request in requestArray
                    index = @values.index(request[0])
                    if values[index] != request[1]
                        isMatching = false
                        break
                    end
                end

                if isMatching
                    result.push(values)
                end
            end
        }
        return result
    end


    # Returnes array of lines from all given tables which match given request
    def self.find(requestArray, tables)
        # Pre
        requestArray = to_s_array(requestArray)

        # Check
        if !requestArray.is_a?(Array)
            raise ArgumentError, "requestArray must be of array type"
        end
        for request in requestArray
            if !request.is_a?(Array) || request.length != 2
                raise ArgumentError, "Request must have following type: [\"valueNameI\", \"valueI\"]"
            end
        end
        if !tables.is_a?(Array)
            raise ArgumentError, "tables must be of array type"
        end
        # TODO rework requestArray look

        # Main
        result = []
        for table in tables
            # TODO search
        end
        return result
    end
end


# Creating DB
usersAmount = 1000
groupsAmount = 300
membershipsAmount = Integer(usersAmount * (rand + 1))
puts "users : #{usersAmount}, groups: #{groupsAmount}, memberships: #{membershipsAmount}"

users = Table.new("UsersDB", ["user_id", "user_name", "user_birth"])
for i in (1..usersAmount)
    users.pushLine(["u" + i.to_s, "User #" + i.to_s, random_date])
end

groups = Table.new("GroupsDB", ["group_id", "user_id", "group_name"])
for i in (1..groupsAmount)
    groups.pushLine(["g" + i.to_s, "u" + rand(usersAmount + 1).to_s, "Group #" + i.to_s])
end

groupsMembers = Table.new("GroupMembersDB", ["group_id", "user_id"])
for i in (1..membershipsAmount)
    groupsMembers.pushLineIfUniq(["g" + rand(groupsAmount + 1).to_s, "u" + rand(usersAmount + 1).to_s])
end

# Finding some lines in single table
puts "Users in group 42:"
for v in groupsMembers.find([["group_id", "g42"]])
    puts v[1]
end

# Finding lines in several tables
puts "Users in groups with user 1 as admin:"
for v in Table::find() # TODO What must be here?
    puts v
end
