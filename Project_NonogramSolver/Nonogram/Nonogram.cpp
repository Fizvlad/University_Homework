#include <iostream>
#include <vector>
#include <cstdlib>

const short _CELL_FILLED = 1,
            _CELL_EMPTY = 0,
            _CELL_UNKNOWN = 2;

class Nonogram
{
    std::vector < std::vector <unsigned short> > _table;
    std::vector < std::vector <unsigned> > _lines_data;
    std::vector < std::vector <unsigned> > _columns_data;

public:
    void print(bool ifPrintData = true)
    {
        unsigned maxLineGroupSize = 0,
                 maxColumnGroupSize = 0;
        if (ifPrintData) {
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
        std::cout << std::endl;
        for (unsigned i = 0; i < maxColumnGroupSize; i++) {
            // Top part
            for (unsigned j = 0; j < maxLineGroupSize; j++) {
                std::cout << "  ";
            }
            for (unsigned j = 0; j < width(); j++) {
                if (i >= maxColumnGroupSize - _columns_data[j].size()) {
                    std::cout << _columns_data[j][i + _columns_data[j].size() - maxColumnGroupSize];
                } else {
                    std::cout << ".";
                }
            }
            std::cout << std::endl;
        }
        for (unsigned i = 0; i < height(); i++) {
            for (unsigned j = 0; j < maxLineGroupSize; j++) {
                if (j >= maxLineGroupSize - _lines_data[i].size()) {
                    std::cout << _lines_data[i][j - maxLineGroupSize + _lines_data[i].size()] << " ";
                } else {
                    std::cout << ". ";
                }
            }
            for (unsigned j = 0; j < width(); j++) {
                char c = ' ';
                switch (_table[i][j]) {
                case _CELL_FILLED:
                    c = '#';
                break;
                case _CELL_EMPTY:
                    c = ' ';
                break;
                case _CELL_UNKNOWN:
                    c = '?';
                break;
                }
                std::cout << c;
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
    Nonogram(std::vector < std::vector <unsigned> > l_data, std::vector < std::vector <unsigned> > c_data)
    {
        _lines_data = l_data;
        _columns_data = c_data;
        _table = vector < vector <unsigned short> (width(), _CELL_UNKNOWN)>
    }
};
