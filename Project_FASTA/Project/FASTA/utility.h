#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include <cstdlib>
#include <cstdio>

long indexOf(char* base, char* targ, unsigned from = 0); //Returns position of first substring equal to targ in base. -1 if was not found

int maximum(int a, int b, int c); //Maximum of three int

long find(int t, int* array, long size); //Return position of symbol t in array. -1 if was not found

char* fgetw(char* string, int size, FILE* file, char splitter = ' '); //Reading symbols from file until meet splitter or EOF or '\n' or until read size - 1 symbol. Read symbols are saved into string. String returned on success, NULL otherwise

void closeFile(FILE* file); //Closing file. Throws error and message on failure

unsigned getStrLength(char* in); //Return size of string

char* copyStr(char* in, unsigned long length = 0); //Copying input string if length is 0 and length of symbols otherwise. Return string

char* merge(char* in1, char* in2); //Merging strings, deleting them and return new one

char* merge(unsigned amount, char* in, ...); //Merging strings, deleting them and return new one

#include "utility.cpp"

#endif // UTILITY_H_INCLUDED
