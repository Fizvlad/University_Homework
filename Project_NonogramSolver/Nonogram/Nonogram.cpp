#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <algorithm>

class Nonogram
{
    const unsigned short _CELL_VALUE_EMPTY = 0, // Usually used to fill cells
                         _CELL_VALUE_FILLED = 1,
                         _CELL_VALUE_UNKNOWN = 2,
                         _CELL_VALUE_CHECK_EMPTY = 3, // Special values for go through
                         _CELL_VALUE_CHECK_FILLED = 4,
                         _CELL_VALUE_CHECK_CONFLICT = 5,

                         _CELL_SIZE = 2; // Used in beauty print only

    bool _solved;
    bool _ifBadData;

    std::vector < std::vector <unsigned short> > _table;

    std::vector < std::vector <unsigned> > _lines_data; // Groups sizes
    std::vector < std::vector <unsigned> > _columns_data;

    std::vector <unsigned> _lines_indexes_difficultyAscending; // Indexes in group size ascending order. To optimize going through lines and columns
    std::vector <unsigned> _columns_indexes_difficultyAscending;

    void _goTrough_ShiftIntervalsFrom (unsigned index,  std::vector <unsigned short> &line, const std::vector <unsigned> &tableLine_data, std::vector <unsigned> groupsIntervals) // Used in go through. Taking all previous intervals as constant. Checking all possible interval combinations and saving into line
    {
        unsigned shiftableGroupsMinimumLength = 0; // Minimum length (Not including previous to interval group)
        for (unsigned i = index; i < tableLine_data.size(); i++) {
            shiftableGroupsMinimumLength += 1; // Minimum space before group
            shiftableGroupsMinimumLength += tableLine_data[i]; // Adding size of group
        }
        if (index == 0) {
            // We have counted space before first group which can be 0
            shiftableGroupsMinimumLength--;
        }

        unsigned amountOfFixedCells = 0; // Amount of fixed cells on left
        for (unsigned i = 0; i < index; i++) {
            amountOfFixedCells += tableLine_data[i]; // Adding sizes of groups
            amountOfFixedCells += groupsIntervals[i]; // Adding intervals before them
        }

        unsigned initialInterval = groupsIntervals[index];
        unsigned maximumPossibleShift = line.size() - amountOfFixedCells - shiftableGroupsMinimumLength; // > 0. Because we checked data in constructor
        for (unsigned shift = 0; shift <= maximumPossibleShift; shift++) {
            // Shifting first available group. Recursively calling for next interval OR checking combination if this is last interval
            groupsIntervals[index] = initialInterval + shift;

            // Checking to prevent conflicts with already saved data
            // Enough to check current interval and group next to it for first intervals (because its recursion)
            // Also need to check space after last group if index == tableLine_data.size() - 1
            bool ifConflict = false;
            for (unsigned i = 0; i < groupsIntervals[index] + tableLine_data[index]; i++) {
                if (i < groupsIntervals[index] && line[amountOfFixedCells + i] == _CELL_VALUE_FILLED) {
                    ifConflict = true;
                    break;
                }
                if (i >= groupsIntervals[index] && line[amountOfFixedCells + i] == _CELL_VALUE_EMPTY) {
                    ifConflict = true;
                    break;
                }
            }
            if (index == tableLine_data.size() - 1) {
                // Checking space after last group
                for (unsigned i = amountOfFixedCells + groupsIntervals[index] + tableLine_data[index]; i < line.size(); i++) {
                    if (line[i] == _CELL_VALUE_FILLED) {
                        ifConflict = true;
                        break;
                    }
                }
            }
            if (ifConflict) {
                // Mismatch with known cells. Current shift is impossible
                continue;
            }

            // No conflicts. Proceed
            if (index != tableLine_data.size() - 1) {
                _goTrough_ShiftIntervalsFrom(index + 1, line, tableLine_data, groupsIntervals);
            } else {
                // Last interval
                // Creating current line
                std::vector <unsigned short> currentLine(line.size(), _CELL_VALUE_CHECK_EMPTY); // Line for current intervals
                unsigned currPos = 0;
                for (unsigned i = 0; i < groupsIntervals.size(); i++) {
                    currPos += groupsIntervals[i];
                    for (unsigned j = 0; j < tableLine_data[i]; j++) {
                        currentLine[currPos] = _CELL_VALUE_CHECK_FILLED;
                        currPos++;
                    }
                }
                // Comparing
                for (unsigned i = 0; i < line.size(); i++) {
                    if (line[i] == _CELL_VALUE_FILLED || line[i] == _CELL_VALUE_EMPTY) {
                        // This cell is already filled. Skipping
                    } else if (line[i] == _CELL_VALUE_UNKNOWN) {
                        // First check. Simply adding our data
                        line[i] = currentLine[i];
                    } else if (line[i] == _CELL_VALUE_CHECK_CONFLICT) {
                        // Conflict (May be filled or empty). Skipping
                    } else if (line[i] != currentLine[i]) {
                        // Conflict with previous variant
                        line[i] = _CELL_VALUE_CHECK_CONFLICT;
                    } else {
                        // Match. If all the lines will match in this cell. Will take as right value
                    }
                }
            }
        }
    }

    bool _goThroughLines_byDifficulty (unsigned d = 0) // Goes through lines with chosen size. Return true if changed _table
    {
        bool ifChanged = false;

        for (unsigned i_ = 0; i_ < height(); i_++) {
            unsigned i = _lines_indexes_difficultyAscending[i_]; // Real index of line
            if (d != 0 && d != _lines_data[i].size()) {
                // Zero means any difficulty
                continue;
            }
            std::cout << "Checking line #" << i << ". Groups amount : " << _lines_data[i].size() << std::endl;
            if (_lines_data[i].size() == 0) {
                continue; // Should not happen due game rules
            }
            std::vector <unsigned> groupsIntervals (_lines_data[i].size(), 1); // groupsIntervals[i] = amount of cells between group #i-1 (or left corner) and group #i
            groupsIntervals[0] = 0; // First group
            std::vector <unsigned short> line = _table[i]; // Copy of table line. Will be filling with values to determine empty or filled cells
            _goTrough_ShiftIntervalsFrom(0, line, _lines_data[i], groupsIntervals);
            // Analyzed line
            for (unsigned j = 0; j < line.size(); j++) {
                if (line[j] == _CELL_VALUE_CHECK_FILLED) {
                    _table[i][j] = _CELL_VALUE_FILLED;
                    ifChanged = true;
                }
                if (line[j] == _CELL_VALUE_CHECK_EMPTY) {
                    _table[i][j] = _CELL_VALUE_EMPTY;
                    ifChanged = true;
                }
            }
        }
        return ifChanged;
    }

    bool _goThroughColumns_byDifficulty (unsigned d = 0) // Goes through  columns with chosen size. Return true if changed _table
    {
        bool ifChanged = false;

        for (unsigned i_ = 0; i_ < width(); i_++) {
            unsigned i = _columns_indexes_difficultyAscending[i_]; // Real index of column
            if (d != 0 && d != _columns_data[i].size()) {
                // Zero means any difficulty
                continue;
            }
            std::cout << "Checking column #" << i << ". Groups amount : " << _columns_data[i].size() << std::endl;
            if (_columns_data[i].size() == 0) {
                continue; // Should not happen due game rules
            }
            std::vector <unsigned> groupsIntervals (_columns_data[i].size(), 1); // groupsIntervals[i] = amount of cells between group #i-1 (or left corner) and group #i
            groupsIntervals[0] = 0; // First group
            std::vector <unsigned short> line (height()); // Copy of table column. Will be filling with values to determine empty or filled cells
            for (unsigned j = 0; j < line.size(); j++) {
                line[j] = _table[j][i];
            }
            _goTrough_ShiftIntervalsFrom(0, line, _columns_data[i], groupsIntervals);

            // Analyzed line
            for (unsigned j = 0; j < line.size(); j++) {
                if (line[j] == _CELL_VALUE_CHECK_FILLED) {
                    _table[j][i] = _CELL_VALUE_FILLED;
                    ifChanged = true;
                }
                if (line[j] == _CELL_VALUE_CHECK_EMPTY) {
                    _table[j][i] = _CELL_VALUE_EMPTY;
                    ifChanged = true;
                }
            }
        }
        return ifChanged;
    }

    bool _ifCorrect (const std::vector <unsigned short> &line, const std::vector <unsigned> &groups, bool ifDebug = false) // Checking given line or column
    {
        unsigned groupIndex = 0;
        unsigned groupSize = 0;
        for (unsigned i = 0; i < line.size(); i++) {
            if (line[i] == _CELL_VALUE_EMPTY) {
                if (groupSize > 0) {
                    // Group ended
                    groupIndex++;
                    groupSize = 0;
                } else {
                    // Group haven't started yet
                }
            } else if (line[i] == _CELL_VALUE_FILLED) {
                if (groupSize > 0) {
                    // Group continues
                    groupSize++;
                    if (groupSize > groups[groupIndex]) {
                        // Group is too big
                        if (ifDebug) {
                            std::cout << "Group is too large. Group index: " << groupIndex << std::endl;
                        }
                        return false;
                    }
                } else {
                    // Group just started
                    groupSize++;
                    if (groupIndex >= groups.size()) {
                        // No such group
                        if (ifDebug) {
                            std::cout << "Too many groups. Group index: " << groupIndex << std::endl;
                        }
                        return false;
                    }
                }
            } else {
                if (ifDebug) {
                    std::cout << "Unknown cell. Group index: " << groupIndex << std::endl;
                }
                return false;
            }
        }
        if (groupSize != 0) {
            // Last group ended
            groupSize = 0;
            groupIndex++;
        }
        if (groupIndex != groups.size()) {
            // Wrong amount of groups
            if (ifDebug) {
                std::cout << "Not enough groups. Group index: " << groupIndex << std::endl;
            }
            return false;
        }
        return true;
    }

    bool _ifLineCorrect (unsigned index) // Checking line by its index
    {
        return _ifCorrect (_table[index], _lines_data[index]);
    }

    bool _ifColumnCorrect (unsigned index) // Checking column by its index
    {
        std::vector <unsigned short> column (height());
        for (unsigned i = 0; i < column.size(); i++) {
            column[i] = _table[i][index];
        }
        return _ifCorrect(column, _columns_data[index]);
    }

    bool _dataCheck () // Checking if this groups are possible
    {
        for (unsigned i = 0; i < height(); i++) {
            unsigned minimum = 0;
            for (unsigned j = 0; j < _lines_data[i].size(); j++) {
                minimum += _lines_data[i][j];
            }
            minimum += _lines_data[i].size() - 1;
            if (minimum > width()) {
                return false;
            }
        }

        for (unsigned i = 0; i < width(); i++) {
            unsigned minimum = 0;
            for (unsigned j = 0; j < _columns_data[i].size(); j++) {
                minimum += _columns_data[i][j];
            }
            minimum += _columns_data[i].size() - 1;
            if (minimum > height()) {
                return false;
            }
        }
        return true;
    }

    Nonogram () // Not allowing empty constructor
    {}
public:

    void print (bool ifBeautify = false, bool ifShowOnUnsolved = false) // Printing to std::cout. Will print "impossible" if unsolvable *or haven't been solved yet*
    {
        if (!ifSolved() || _ifBadData) {
            std::cout << "impossible" << std::endl;
            if (!ifShowOnUnsolved) {
                return;
            }
        }
        unsigned maxLineGroupSize = 0,
                 maxColumnGroupSize = 0;
        if (ifBeautify) {
            // Counting sizes of fields with given data
            for (unsigned i = 0; i < _lines_data.size(); i++) {
                if (_lines_data[i].size() > maxLineGroupSize) {
                    maxLineGroupSize = _lines_data[i].size();
                }
            }
            for (unsigned i = 0; i < _columns_data.size(); i++) {
                if (_columns_data[i].size() > maxColumnGroupSize) {
                    maxColumnGroupSize = _columns_data[i].size();
                }
            }
        }

        // If don't need beauty output input data won't be printed
        for (unsigned i = 0; i < maxColumnGroupSize; i++) {
            // Top part
            for (unsigned j = 0; j < maxLineGroupSize; j++) {
                std::cout << std::setw(_CELL_SIZE + 1) << ""; // Skipping space in top left corner
            }
            for (unsigned j = 0; j < width(); j++) {
                // Printing given data
                if (i >= maxColumnGroupSize - _columns_data[j].size()) {
                    std::cout << std::setw(_CELL_SIZE) << _columns_data[j][i + _columns_data[j].size() - maxColumnGroupSize];
                } else {
                    std::cout << std::setw(_CELL_SIZE) << ".";
                }
            }
            std::cout << std::endl;
        }
        for (unsigned i = 0; i < height(); i++) {
            for (unsigned j = 0; j < maxLineGroupSize; j++) {
                // Printing given data
                if (j >= maxLineGroupSize - _lines_data[i].size()) {
                    std::cout << std::setw(_CELL_SIZE + 1) << _lines_data[i][j - maxLineGroupSize + _lines_data[i].size()]; // Adding one space
                } else {
                    std::cout << std::setw(_CELL_SIZE + 1) << ".";
                }
            }
            for (unsigned j = 0; j < width(); j++) {
                char c = ' ';
                unsigned short cellValue = _table[i][j];
                if (cellValue == _CELL_VALUE_FILLED) {
                    c = '#';
                } else if (cellValue == _CELL_VALUE_EMPTY) {
                    c = ' ';
                } else if (cellValue == _CELL_VALUE_UNKNOWN) {
                    c = '?';
                }
                std::cout << std::setw( ifBeautify?_CELL_SIZE:1 ) << c;
            }
            std::cout << std::endl;
        }
    }

    unsigned width () // Return width of table
    {
        return _columns_data.size();
    }

    unsigned height () // Return height of table
    {
        return _lines_data.size();
    }

    void solve () // Filling table
    {
        if (_ifBadData) {
            return;
        }
        unsigned maximumGroupsAmount = _lines_data[_lines_indexes_difficultyAscending[_lines_indexes_difficultyAscending.size() - 1]].size();
        unsigned maximumColumnGroupsAmount = _columns_data[_columns_indexes_difficultyAscending[_columns_indexes_difficultyAscending.size() - 1]].size();
        if (maximumColumnGroupsAmount > maximumGroupsAmount) {
            maximumGroupsAmount = maximumColumnGroupsAmount;
        }
        bool ifChanged = true;
        while (ifChanged) {
            // Doing while at least one of methods gives changes
            ifChanged = false;
            for (unsigned i = 1 ; i <= maximumGroupsAmount; i++) {
                ifChanged = ifChanged || _goThroughLines_byDifficulty(i);
                ifChanged = ifChanged || _goThroughColumns_byDifficulty(i);
            }
        }
        return;
    }

    bool ifSolved () // Checking if this nonogram is solved
    {
        if (_ifBadData) {
            return false; // Unsolvable in this case
        }
        if (_solved) {
            return true; // No need to determine it twice
        }

        for (unsigned i = 0; i < height(); i++) {
            if (!_ifLineCorrect(i)) {
                return false;
            }
        }
        for (unsigned i = 0; i < width(); i++) {
            if (!_ifColumnCorrect(i)) {
                return false;
            }
        }
        _solved = true;
        return true;
    }

    unsigned short cell (unsigned line, unsigned column) // Return cell data
    {
        return _table[line][column];
    }

    Nonogram (const std::vector < std::vector <unsigned> > &l_data, const std::vector < std::vector <unsigned> > &c_data) // Constructor
    {
        _lines_data = l_data;
        _columns_data = c_data;
        std::vector < std::pair <unsigned, unsigned> > tmp; // Temporary array for sorting of lines and columns

        // Sorting lines. Difficulty = amount of groups
        for (unsigned i = 0; i < _lines_data.size(); i++) {
            tmp.push_back(std::pair <unsigned, unsigned> (_lines_data[i].size(), i)); // Amount of groups, index
        }
        std::sort(tmp.begin(), tmp.end()); // Will be sorted by first element of pair
        _lines_indexes_difficultyAscending = std::vector <unsigned> (tmp.size());
        for (unsigned i = 0; i < _lines_indexes_difficultyAscending.size(); i++) {
            _lines_indexes_difficultyAscending[i] = tmp[i].second;
        }

        tmp.clear();
        // Sorting columns
        for (unsigned i = 0; i < _columns_data.size(); i++) {
            tmp.push_back(std::pair <unsigned, unsigned> (_columns_data[i].size(), i)); // Amount of groups, index
        }
        std::sort(tmp.begin(), tmp.end()); // Will be sorted by first element of pair
        _columns_indexes_difficultyAscending = std::vector <unsigned> (tmp.size());
        for (unsigned i = 0; i < _columns_indexes_difficultyAscending.size(); i++) {
            _columns_indexes_difficultyAscending[i] = tmp[i].second;
        }

        for (unsigned i = 0; i < height(); i++) {
            std::vector <unsigned short> line (width(), _CELL_VALUE_UNKNOWN);
            _table.push_back(line);
        }
        _solved = false;
        _ifBadData = !_dataCheck(); // Checking
    }
};
