#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include "utility.cpp"

long min(long a, long b, long c);
long min(long a, long b, long c, long d);
long max(long a, long b, long c);
long max(long a, long b, long c, long d);

char* floatToString(float in);

char* merge(char* in1, char* in2); // Merging strings and deleting initial ones
char* merge(unsigned amount, char* in, ...); // Merging lot of strings and deleting initial ones


#endif // UTILITY_H_INCLUDED
