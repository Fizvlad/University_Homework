#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>

class Nonogram
{
    const unsigned short _CELL_VALUE_EMPTY = 0, // Usually used to fill cells
                         _CELL_VALUE_FILLED = 1,
                         _CELL_VALUE_UNKNOWN = 2,
                         _CELL_VALUE_CHECK_EMPTY = 3, // Special values for go through
                         _CELL_VALUE_CHECK_FILLED = 4,
                         _CELL_VALUE_CHECK_CONFLICT = 5,

                         _CELL_SIZE = 2; // Used in beauty print only

    std::vector < std::vector <unsigned short> > _table;

    std::vector < std::vector <unsigned> > _lines_data; // Groups sizes
    std::vector < std::vector <unsigned> > _columns_data;

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
            // Checking length
            if (shiftableGroupsMinimumLength > line.size()) {
                // Impossible situation. Not checking
                return;
            }
        }

        unsigned amountOfFixedCells = 0; // Amount of fixed cells on left
        for (unsigned i = 0; i < index; i++) {
            amountOfFixedCells += tableLine_data[i]; // Adding sizes of groups
            amountOfFixedCells += groupsIntervals[i]; // Adding intervals before them
        }

        unsigned initialInterval = groupsIntervals[index];
        unsigned maximumPossibleShift = line.size() - amountOfFixedCells - shiftableGroupsMinimumLength;
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

    bool _goThroughLines () // Goes through lines. Return true if changed _table
    {
        bool ifChanged = false;
        for (unsigned i = 0; i < height(); i++) {
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

    bool _goThroughColumns() // Goes through  columns. Return true if changed _table
    {
        bool ifChanged = false;
        for (unsigned i = 0; i < width(); i++) {
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

    bool _ifLineCorrect(unsigned index) // Checking line by its index
    {
        unsigned groupIndex = 0;
        unsigned groupSize = 0;
        for (unsigned i = 0; i < width(); i++) {
            if (_table[index][i] == _CELL_VALUE_EMPTY) {
                if (groupSize > 0) {
                    // Group ended
                    groupIndex++;
                    groupSize = 0;
                } else {
                    // Group haven't started yet
                }
            } else if (_table[index][i] == _CELL_VALUE_FILLED) {
                if (groupSize > 0) {
                    // Group
                    groupSize++;
                    if (groupSize > _lines_data[index][groupIndex]) {
                        // Group is too big
                        std::cout << "Group is too large. Line #" << index << ", group index: " << groupIndex << std::endl;
                        return false;
                    }
                } else {
                    // Group just started
                    groupSize++;
                    if (groupIndex >= _lines_data[index].size()) {
                        // No such group
                        std::cout << "Too many groups. Line #" << index << ", group index: " << groupIndex << std::endl;
                        return false;
                    }
                }
            } else {
                std::cout << "Unknown cell. Line #" << index << ", group index: " << groupIndex << std::endl;
                return false;
            }
        }
        if (groupSize != 0) {
            // Last group ended
            groupSize = 0;
            groupIndex++;
        }
        if (groupIndex != _lines_data[index].size()) {
            // Wrong amount of groups
            std::cout << "Not enough groups. Line #" << index << ", group index: " << groupIndex << std::endl;
            return false;
        }
        return true;
    }

    bool _ifColumnCorrect(unsigned index) // Checking column by its index
    {
        return true;
    }
public:
    void print (bool ifBeautify = false, bool ifShowOnUnsolved = false)
    {
        if (!check()) {
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

    unsigned width ()
    {
        return _columns_data.size();
    }

    unsigned height ()
    {
        return _lines_data.size();
    }

    void solve ()
    {
        bool ifChanged = true;
        while (ifChanged) {
            // Doing while at least one of methods gives changes
            ifChanged = _goThroughLines() || _goThroughColumns();
        }
    }

    unsigned short cell (unsigned line, unsigned column)
    {
        return _table[line][column];
    }

    bool check ()
    {
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
        return true;
    }

    Nonogram (std::vector < std::vector <unsigned> > l_data, std::vector < std::vector <unsigned> > c_data)
    {
        _lines_data = l_data;
        _columns_data = c_data;
        for (unsigned i = 0; i < height(); i++) {
            std::vector <unsigned short> line (width(), _CELL_VALUE_UNKNOWN);
            _table.push_back(line);
        }
    }
};
