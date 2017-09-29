#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>

const short _CELL_VALUE_FILLED = 1,
            _CELL_VALUE_EMPTY = 0,
            _CELL_VALUE_UNKNOWN = 2;

const unsigned short _CELL_SIZE = 2;

class Nonogram
{
    std::vector < std::vector <unsigned short> > _table;
    std::vector < std::vector <unsigned> > _lines_data;
    std::vector < std::vector <unsigned> > _columns_data;

    bool _goThroughLines () // Goes through  lines. Return true if changed _table
    {
        bool ifChanged = false;
        for (unsigned i = 0; i < height(); i++) {
            if (_lines_data[i].size() == 0) {
                // Can not normally happen due to rules but better take into account
                for (unsigned j = 0; j < width(); j++) {
                    _table[i][j] = _CELL_VALUE_EMPTY;
                }
                ifChanged = true;
            }
            if (_lines_data[i].size() == 1 && width() - _lines_data[i][0] < _lines_data[i][0]) {
                for (unsigned j = width() - _lines_data[i][0]; j < _lines_data[i][0]; j++) {
                    _table[i][j] = _CELL_VALUE_FILLED;
                }
                ifChanged = true;
            }
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
                switch (_table[i][j]) {
                case _CELL_VALUE_FILLED:
                    c = '#';
                break;
                case _CELL_VALUE_EMPTY:
                    c = ' ';
                break;
                case _CELL_VALUE_UNKNOWN:
                    c = '?';
                break;
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
        _goThroughLines();
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
