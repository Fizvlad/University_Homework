#include <cstdlib>
#include <cstdio>

#include "utility.h"

long indexOf(char* base, char* targ, unsigned from)
{
    /*
        Returns position of first substring equal to targ in base.
        -1 if was not found
    */

    try {
        if (base == NULL || targ == NULL) {
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

    long out = from; // Symbol of base
    unsigned k = 0; // Symbol of targ
    while (base[out] != '\0' && targ[k] != '\0') {
        if (base[out] == targ[k]) {
            k++;
        } else {
            k = 0;
        }
        out++;
        if (targ[k] == '\0') {
            return out - k;
        }
    }
    return -1;
}

int maximum(int a, int b, int c)
{
    /*
        Maximum of three int
    */
    int t = a;
    if (b > a) {
        t = b;
    }
    if (c > t) {
        return c;
    }
    return t;
}

long find(const int t, const int* array, const long size)
{
    /*
        Return position of symbol t in array
        -1 if was not found
    */
    for (long i = 0; i < size; i++) {
        if (array[i] == t) {
            return i;
        }
    }
    return -1;
}

char* fgetw(char* string, int size, FILE* file, char splitter)
{
    /*
        Reading symbols from file until meet splitter or EOF or '\n' or until read size - 1 symbol.
        Read symbols are saved into string
        String returned on success, NULL otherwise
    */
    if (size <= 1 || file == NULL || string == NULL) {
        return NULL;
    }
    int i = 0;
    int ch = getc(file);
    if (ch == EOF) {
        return NULL;
    }
    while ((unsigned char)ch != splitter && (unsigned char)ch != '\n' && ch != EOF && i < size - 2) {
        string[i] = (unsigned char)ch;
        i++;
        ch = getc(file);
    }
    // Now ch = -1 OR splitter OR '\n' OR last char to be saved
    if (ch == EOF) {
        string[i] ='\0';
    } else {
        string[i] = (unsigned char)ch;
        string[i + 1] = '\0';
    }
    return string;
}

void closeFile(FILE* file)
{
    /*
        Closing file. Throws error and message on failure
    */
    try {
        // Closing file
        int re = fclose(file);
        if (re != 0) {
            throw -12;
        }
    } catch (int error_code_) {
        switch (error_code_) {
        case -12:
            fprintf(stderr, "%s", "    !Error: Unable to close file\n");
            exit(EXIT_FAILURE);
            break;
        default:
            fprintf(stderr, "%s", "    !Error: Unknown error\n");
            exit(EXIT_FAILURE);
        }
    }
}

unsigned getStrLength(char* in)
{
    /*
        Return size of string
    */
    if (in == NULL) {
        return 0;
    }
    unsigned out = 0;
    while(*(in + out) != '\0') {
        try {
            out++;
            // Checking length to prevent overflow
            if (out == -1) {
                throw -11;
            }
        } catch (int error_code) {
            switch(error_code) {
            case -11:
                fprintf(stderr, "%s", "    !Error: String is too big. Returned 0\n");
                return 0;
                break;
            default:
                fprintf(stderr, "%s", "    !Error: Unknown error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    return out;
}

char* copyStr(char* in, unsigned long length)
{
    /*
        Copying input string if length is 0 and length of symbols otherwise.
        Return string
    */
    if (in == NULL) {
        return NULL;
    }
    if (length == 0) {
        length = getStrLength(in);
    }
    char* out = new char[length + 1];
    for (unsigned long i = 0; i < length; i++) {
        out[i] = in[i];
    }
    out[length] = '\0';
    return out;
}

char* merge(char* in1, char* in2)
{
    /*
        Merging strings, deleting them and return new one
    */
    if (in1 == NULL || in2 == NULL) {
        fprintf(stderr, "%s", "    !Error: NULL input. Returned NULL\n");
        return NULL;
    }
    unsigned l1 = getStrLength(in1);
    unsigned l2 = getStrLength(in2);

    char* out = new char[l1 + l2 + 1];
    out[l1 + l2] = '\0';

    unsigned i = 0;
    for (; i < l1; i++) {
        out[i] = in1[i];
    }
    for (; i < l1 + l2; i++) {
        out[i] = in2[i - l1];
    }

    delete [] in1;
    delete [] in2;

    return out;
};

char* merge(unsigned amount, char* in, ...)
{
    /*
        Merging strings, deleting them and return new one
    */
    if (amount == 0) {
        fprintf(stderr, "%s", "    !Error: Wrong input. Returned NULL\n");
        return NULL;
    } else if (amount == 1) {
        return in;
    }
    char* out = new char[1];
    out[0] = '\0';
    char** curr = &in;
    for (unsigned i = 0; i < amount; i++) {
        out = merge(out, *curr);
        curr++;
    }
    return out;
}
