#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>

class Nonogram
{
    const unsigned short _CELL_VALUE_FILLED = 1,
                         _CELL_VALUE_EMPTY = 0,
                         _CELL_VALUE_UNKNOWN = 2,

                         _CELL_VALUE_CHECK_CONFLICT = 3;

    const unsigned short _CELL_SIZE = 2;

    std::vector < std::vector <unsigned short> > _table;
    std::vector < std::vector <unsigned> > _lines_data;
    std::vector < std::vector <unsigned> > _columns_data;

    bool _goTrough_ShiftIntervalsFrom (unsigned index,  std::vector <unsigned short> &tableLine, const std::vector <unsigned> &tableLine_data, std::vector <unsigned> &groupsIntervals) // Used in go through. Taking all previous intervals as constant. Checking all possible interval combinations and saving into line
    {
        bool ifChanged = false; // Return statement
        std::vector <unsigned short> line (tableLine.size(), _CELL_VALUE_UNKNOWN); // Copy of tableLine. Will be filling with values to determine empty or filled cells

        unsigned shiftableGroupsMinimumLength = 0; // Minimum length (Including previous to interval group)
        for (unsigned i = index - 1; i < tableLine_data.size(); i++) {
            shiftableGroupsMinimumLength += tableLine_data[i]; // Adding sizes of groups
        }
        shiftableGroupsMinimumLength += tableLine_data.size() - index; // Minimum space between groups

        unsigned amountOfFixedCells = 0; // Amount of fixed cells on left
        for (unsigned i = 0; i < index - 1; i++) {
            amountOfFixedCells += tableLine_data[i]; // Adding sizes of groups
            amountOfFixedCells += groupsIntervals[i]; // Adding intervals
        }
        amountOfFixedCells += groupsIntervals[index]; // Adding last fixed interval

        for (unsigned i = 0; i < tableLine.size() - amountOfFixedCells - shiftableGroupsMinimumLength; i++) {
            // Shifting first available group. Recursively calling for next interval OR checking combination if this is last interval
        }

        return ifChanged;
    }

    bool _goThroughLines () // Goes through lines. Return true if changed _table
    {
        bool ifChanged = false;
        for (unsigned i = 0; i < height(); i++) {
            std::vector <unsigned> groupsIntervals (_lines_data[i].size(), 1); // groupsIntervals[i] = amount of cells between group #i-1 (or left corner) and group #i
            groupsIntervals[0] = 0; // First group

            _goTrough_ShiftIntervalsFrom(0, _table[i], _lines_data[i], groupsIntervals);
        }
        return ifChanged;
    }

    bool _goThroughColumns() // Goes through  columns. Return true if changed _table
    {
        return false;
    }
public:
    void print(bool ifBeautify = false)
    {
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

    unsigned width()
    {
        return _columns_data.size();
    }

    unsigned height()
    {
        return _lines_data.size();
    }

    void solve()
    {
        bool ifChanged = true;
        while (ifChanged) {
            ifChanged = _goThroughLines() && _goThroughColumns();
        }
    }

    unsigned short cell(unsigned line, unsigned column)
    {
        return _table[line][column];
    }

    Nonogram(std::vector < std::vector <unsigned> > l_data, std::vector < std::vector <unsigned> > c_data)
    {
        _lines_data = l_data;
        _columns_data = c_data;
        for (unsigned i = 0; i < height(); i++) {
            std::vector <unsigned short> line (width(), _CELL_VALUE_UNKNOWN);
            _table.push_back(line);
        }
    }
};
