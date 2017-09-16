#ifndef FASTA_H_INCLUDED
#define FASTA_H_INCLUDED

#include <cstdlib>
#include <cstdio>
#include "utility.h"
#include "sequence.h"

class statistic
{
    /*
        Acceptable symbols described in constructor
    */
    friend class FASTA;
    static const unsigned ACCEPTED_SYMBOLS_AMOUNT_ = 18;
    int ACCEPTED_SYMBOLS_[ACCEPTED_SYMBOLS_AMOUNT_]; // List of acceptable symbols can be found in fasta.cpp
    unsigned symbols_amounts_[ACCEPTED_SYMBOLS_AMOUNT_]; // Contains amount of ACCEPTED_SYMBOLS_[i]

    void clear();
    statistic();
    statistic(const statistic& in);
};

typedef class FASTA
{
    char*         id_; // id
    unsigned      id_length_; // id length
    char*         description_; // description
    unsigned      description_length_; // description length
    sequence      sequence_; // sequence
    statistic     statistic_; // statistic of nucleotide frequency

    void updateStatistics();
public:
    void clear();
    void print (FILE* stream = stdout);
    void printStat (FILE* stream = stdout);
    void save (const char* path);
    void read (const char* path);
    void setId (char* in);
    void setDescription (char* in);
    void setSequence (char* in);
    void align (FASTA in);
    FASTA();
    FASTA (const FASTA& in);
    ~FASTA();
};

#endif // FASTA_H_INCLUDED
