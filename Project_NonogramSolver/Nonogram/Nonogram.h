#ifndef NONOGRAM_H_INCLUDED
#define NONOGRAM_H_INCLUDED

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

#include "Nonogram.h"

class Nonogram
{
    static const unsigned short _CELL_VALUE_EMPTY = 0, // Usually used to fill cells
                                _CELL_VALUE_FILLED = 1,
                                _CELL_VALUE_UNKNOWN = 2,
                                _CELL_VALUE_CHECK_EMPTY = 3, // Special values for go through
                                _CELL_VALUE_CHECK_FILLED = 4,
                                _CELL_VALUE_CHECK_CONFLICT = 5,

                                _CELL_SIZE = 2; // Used in beauty print in << setw(*) <<

    bool _solved; // If solved
    bool _ifBadData; // Result of first check of data in constructor
    unsigned _maximumDifficulty; // Maximum difficulty of row or column. Determined in constructor

    std::vector < std::vector <unsigned short> > _table; // Table with _CELL_VALUE_*

    std::vector < std::vector <unsigned> > _lines_data; // Array of groups sizes
    std::vector < std::vector <unsigned> > _columns_data;

    std::vector < std::pair <unsigned, unsigned> > _lines_indexes_difficultyAscending; // Indexes in difficulty ascending order. Difficulty/Index
    std::vector < std::pair <unsigned, unsigned> > _columns_indexes_difficultyAscending;


    void _goTrough_ShiftIntervalsFrom (unsigned index, std::vector <unsigned short> &line, const std::vector <unsigned> &tableLine_data, std::vector <unsigned> groupsIntervals); // Used in go through. Taking all previous intervals as constant. Checking all possible interval combinations and saving into line
    bool _goThroughLines_byDifficulty (unsigned d = 0); // Goes through lines with chosen difficulty. Return true if changed _table
    bool _goThroughColumns_byDifficulty (unsigned d = 0); // Goes through  columns with chosen difficulty. Return true if changed _table

    bool _ifCorrect (const std::vector <unsigned short> &line, const std::vector <unsigned> &groups, bool ifDebug = false); // Checking given line or column
    bool _ifLineCorrect (unsigned index); // Checking line by its index
    bool _ifColumnCorrect (unsigned index); // Checking column by its index
    bool _dataCheck (); // Checking if this groups are possible

    Nonogram (); // Not allowing empty constructor
public:

    void print (bool ifBeautify = false, bool ifShowOnUnsolved = false); // Printing to std::cout. Will print "impossible" if unsolvable *or haven't been solved yet*. Beauty output: Showing given data (Needs much more space. Better change console). ifShowOnUnsolved: If need to print table if was not solved

    unsigned width (); // Return width of table
    unsigned height (); // Return height of table

    void solve (); // Filling table

    bool ifSolved (); // Checking if this nonogram is solved

    unsigned short cell (unsigned line, unsigned column); // Return cell data

    Nonogram (const std::vector < std::vector <unsigned> > &l_data, const std::vector < std::vector <unsigned> > &c_data); // Constructor. Also determining difficulty and sorting indexes
    // Copying, moving operator= and destructor are usual
};
#endif // NONOGRAM_H_INCLUDED
