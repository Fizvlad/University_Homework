#ifndef TRIAD_H_INCLUDED
#define TRIAD_H_INCLUDED

#include "triad.cpp"

typedef class triad;
const triad& operator + (triad in1, const triad& in2);
const triad& operator - (triad in1, const triad& in2);
const triad& operator * (triad in, const int& i);
const triad& operator * (const int& i, triad in);

#endif // TRIAD_H_INCLUDED
