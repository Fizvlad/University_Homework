#include <cstdlib>
#include <cstdio>
#include "utility.h"

#include "sequence.h"

void sequence::addStr (unsigned amount)
{
    char** newList = new char*[list_size_ + amount];
    for (int i = 0; i < list_size_; i++) {
        newList[i] = list_[i];
    }
    for (int i = 0; i < amount; i++) {
        newList[list_size_ + i] = new char[STRING_SIZE_ + 1];
        newList[list_size_ + i][0] = '\0';
    }
    if (list_ != NULL) {
        delete [] list_;
    }
    list_ = newList;
    list_size_ += amount;
}

void sequence::clear()
{
    if (list_ != NULL) {
        for (unsigned i = 0; i < list_size_; i++) {
            delete [] list_[i];
        }
        delete [] list_;
        list_ = NULL;
    }
    length_ = 0;
    list_size_ = 0;
}

long sequence::indexOf(char* in, unsigned from)
{
    if (list_ == NULL || from > length_) {
        return -1;
    }

    try {
        if (in == NULL) {
            throw -11;
        }
    } catch (int error_code) {
        switch(error_code) {
        case -11:
            fprintf(stderr, "%s", "    !Error: NULL input. Returned -1\n");
            return -1;
            break;
        default:
            fprintf(stderr, "%s", "    !Error: Unknown error\n");
            exit(EXIT_FAILURE);
        }
    }

    long out = from; // Symbol of sequence
    unsigned k = 0; // Symbol of input
    while (out < length_ && in[k] != '\0') {
        if (operator[](out) == in[k]) {
            k++;
        } else {
            k = 0;
        }
        out++;
        if (in[k] == '\0') {
            return out - k;
        }
    }
    return -1;
}

unsigned sequence::getLength()
{
    return length_;
}

void sequence::print (FILE* stream)
{
    for (unsigned i = 0; i < list_size_; i++) {
        fprintf(stream, "%s", list_[i]);
    }
}

char& sequence::operator[](unsigned in)
{
    char nullOut = '\0';

    try {
        if (in >= length_) {
            throw -11;
        }
    } catch (int error_code) {
        switch(error_code) {
        case -11:
            fprintf(stderr, "%s", "    !Error: Out of boundaries\n");
            return nullOut;
            break;
        default:
            fprintf(stderr, "%s", "    !Error: Unknown error\n");
            exit(EXIT_FAILURE);
        }
    }

    unsigned k = in / STRING_SIZE_; // Number of string
    unsigned l = in % STRING_SIZE_; // Number of char in string

    try {
        if (k >= list_size_) {
            throw -12;
        }
        if (l >= STRING_SIZE_) { // Actually that would never happen
            throw -13;
        }
    } catch (int error_code) {
        switch(error_code) {
        case -12:
            fprintf(stderr, "%s%d%c", "    !Error: No string with number ", k, '\n');
            return nullOut;
            break;
        case -13:
            fprintf(stderr, "%s%d%c", "    !Error: No char with number ", l, '\n');
            return nullOut;
            break;
        default:
            fprintf(stderr, "%s", "    !Error: Unknown error\n");
            exit(EXIT_FAILURE);
        }
    }

    return list_[k][l];
}

void sequence::add(char in)
{
    unsigned l = length_ - (list_size_ - 1) * STRING_SIZE_; // Size of last string
    if (l != STRING_SIZE_) { // Last string is not full yet
        list_[list_size_ - 1][l] = in;
        list_[list_size_ - 1][l + 1] = '\0';
    } else { // Have to create new string because last one is full
        addStr(1);
        list_[list_size_ - 1][0] = in;
        list_[list_size_ - 1][1] = '\0';
    }
    length_++;
}

void sequence::add(char* in)
{

    try {
        if (in == NULL) {
            throw -11;
        }
    } catch (int error_code) {
        switch(error_code) {
        case -11:
            fprintf(stderr, "%s", "    !Error: NULL input\n");
            return;
            break;
        default:
            fprintf(stderr, "%s", "    !Error: Unknown error\n");
            exit(EXIT_FAILURE);
        }
    }

    unsigned l = length_ - (list_size_ - 1) * STRING_SIZE_; // Size of last string
    unsigned t = getStrLength(in);
    if (STRING_SIZE_ - l >= t) { // No need to create new string
        for (unsigned i = 0; i < t; i++) {
            list_[list_size_ - 1][l + i] = in[i];
        }
        list_[list_size_ - 1][l + t] = '\0';
    } else { // Have to create 1+ new strings
        unsigned j = list_size_; // Number of string being filled
        unsigned strNeeded = (t - (STRING_SIZE_ - l)) / STRING_SIZE_;
        if ((t - (STRING_SIZE_ - l)) % STRING_SIZE_ != 0) {
            strNeeded++;
        }
        addStr(strNeeded);
        unsigned k = 0; // Current symbol in input string
        for (; k < STRING_SIZE_ - l; k++) {
            list_[j - 1][l + k] = in[k];
        }
        unsigned i = 0; // Current symbol in string
        for (; j < list_size_; j++) {
            for (; i < STRING_SIZE_ && k < t; i++) {
                list_[j][i] = in[k];
                k++;
            }
            list_[j][i] = '\0';
            if (k != t) {
                i = 0;
            }
        }
    }
    length_ += t;
}

sequence::sequence()
{
    list_ = NULL;
    length_ = 0;
    list_size_ = 0;
}

sequence::sequence (char* in)
{

    try {
        if (in == NULL) {
            throw -11;
        }
    } catch (int error_code) {
        switch(error_code) {
        case -11:
            fprintf(stderr, "%s", "    !Error: NULL input\n");
            length_ = 0;
            list_size_ = 0;
            return;
            break;
        default:
            fprintf(stderr, "%s", "    !Error: Unknown error\n");
            exit(EXIT_FAILURE);
        }
    }

    length_ = getStrLength(in);
    list_size_ = length_ / STRING_SIZE_;
    if (length_ % STRING_SIZE_ != 0) { // Need one more string which will have less than STRING_SIZE_ symbols
        list_size_++;
    }
    list_ = new char*[list_size_];

    unsigned k = 0; // number of symbol currently being copied from in
    for (unsigned i = 0; i < list_size_; i++) {
        list_[i] = new char[STRING_SIZE_ + 1];
        unsigned j = 0; // number of current symbol in current string
        for (; j < STRING_SIZE_ && k < length_; j++) {
            list_[i][j] = in[k];
            k++;
        }
        list_[i][j] = '\0'; // when stopped copying (when j == STRING_SIZE_ || k == length_)
    }
}

sequence::sequence (const sequence& in)
{
    length_ = in.length_;
    list_size_ = in.list_size_;
    list_ = new char*[list_size_];
    for (unsigned i = 0; i < list_size_; i++) {
        list_[i] = copyStr(in.list_[i], STRING_SIZE_);
    }
}

sequence::~sequence()
{
    clear();
}

