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
            # Used to write values in first line but its is useless and harmful
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
                if line.strip == strForSearch
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
                    if index == nil
                        next
                    end
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
    def self.find(requestArray, tables, connections)
        # Pre
        requestArray = to_s_array(requestArray)
        connections = to_s_array(connections)

        # Check
        if !requestArray.is_a?(Array)
            raise ArgumentError, "requestArray must be of array type"
        end
        for request in requestArray
            if !request.is_a?(Array) || request.length != 3
                raise ArgumentError, "Request must have following type: [tableIndex, \"valueNameI\", \"valueI\"]"
            end
        end
        if !tables.is_a?(Array) || tables.length == 0
            raise ArgumentError, "tables must be a not empty array"
        end
        if !connections.is_a?(Array)
            raise ArgumentError, "connections must be of array type"
        end
        for connection in connections
            if !connection.is_a?(Array) || connection.length != 4
                raise ArgumentError, "Connection must have following type: [firstTableIndex, \"valueName1\", secondTableIndex, \"valueName2\"]"
            end
        end

        # Main
        tableResults = []
        for i in (0...tables.length)
            tableRequest = []
            for request in requestArray
                if request[0].to_i == i
                    # This request is for current table
                    tableRequest.push([request[1], request[2]])
                end
            end
            tableResults.push(tables[i].find(tableRequest))
        end

        result = []
        for i in (0...tableResults[0].length)
            fullLines = util_recursive_findInConnections(tables, tableResults, connections, 0, i) # Array with matches to given line
            for fullLine in fullLines
                isFull = true
                for i in (0...tables.length)
                    if fullLine[i] == nil
                        isFull = false
                        break
                    end
                end
                if isFull
                    result.push(fullLine)
                end
            end
        end
        return result
    end

    def util_value_index(valueName)
        return @values.index(valueName)
    end

    private
    def self.util_recursive_findInConnections(initialTables, filteredTables, connections, here_tableIndex, here_lineIndex, visitedTables = [])
        results = []
        for connection in connections
            ci = connection.index(here_tableIndex.to_s)
            if ci == nil
                # This connection is not related to current table
                next
            end
            here_valueName = connection[ci + 1]
            here_valueIndex = initialTables[here_tableIndex].util_value_index(here_valueName)
            here_value = filteredTables[here_tableIndex][here_lineIndex][here_valueIndex]

            there_tableIndex = connection[ci - 2].to_i
            if visitedTables.index(there_tableIndex) != nil
                # Already visited this table
                next
            end
            there_valueName = connection[ci - 1]
            there_valueIndex = initialTables[there_tableIndex].util_value_index(there_valueName)

            visitedTables.push(here_tableIndex).uniq! # Adding this table to visited
            for there_lineIndex in (0...filteredTables[there_tableIndex].length)
                # Going through lines in another table
                there_value = filteredTables[there_tableIndex][there_lineIndex][there_valueIndex]
                if here_value == there_value
                    # Found matching one
                    recursive_results = util_recursive_findInConnections(initialTables, filteredTables, connections, there_tableIndex, there_lineIndex, visitedTables)
                    if recursive_results.length == 0
                        # All connections were visited. Just adding matching line and this line to current results
                        result = []
                        result[here_tableIndex] = filteredTables[here_tableIndex][here_lineIndex]
                        result[there_tableIndex] = filteredTables[there_tableIndex][there_lineIndex]
                        results.push(result)
                    else
                        # Recursively got results. Adding this line data
                        results = recursive_results
                        for result in results
                            result[here_tableIndex] = filteredTables[here_tableIndex][here_lineIndex]
                        end
                    end
                end
            end
        end
        return results
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

groups = Table.new("GroupsDB", ["group_id", "admin_id", "group_name"])
userIndex = 0 # For future selections saving last admin id
for i in (1..groupsAmount)
    userIndex = rand(usersAmount + 1) + 1
    groups.pushLine(["g" + i.to_s, "u" + userIndex.to_s, "Group #" + i.to_s])
end

groupsMembers = Table.new("GroupMembersDB", ["group_id", "member_id"])
for i in (1..membershipsAmount)
    groupsMembers.pushLineIfUniq(["g" + (rand(groupsAmount + 1) + 1).to_s, "u" + (rand(usersAmount + 1) + 1).to_s])
end


# Finding some lines in single table
puts "Groups with user #{userIndex} as admin:"
selection1 = groups.find([["admin_id", "u" + userIndex.to_s]])
for v in selection1
    puts "#{v}"
end

# Finding lines in several tables
puts "Users in groups with user #{userIndex} as admin:"
selection2 = Table::find(
        [[1, "admin_id", "u" + userIndex.to_s]],
        [users, groups, groupsMembers],
        [[0, "user_id", 2, "member_id"], [2, "group_id", 1, "group_id"]])
for r in selection2
    puts "#{r}"
end
