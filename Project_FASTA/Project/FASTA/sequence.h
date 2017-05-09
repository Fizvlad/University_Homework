#ifndef SEQUENCE_H_INCLUDED
#define SEQUENCE_H_INCLUDED

typedef class sequence
{
    char** list_;
    unsigned length_;
    unsigned list_size_; // Amount of strings
    static const unsigned STRING_SIZE_ = 72; // Size of each string
    void addStr (unsigned amount);
public:
    void clear();
    long indexOf(char* in, unsigned from = 0);
    unsigned getLength();
    void print (FILE* stream = stdout);
    char& operator[](unsigned in);
    void add(char in);
    void add(char* in);
    sequence();
    sequence (char* in);
    sequence (const sequence& in);
    ~sequence();
};

#endif // SEQUENCE_H_INCLUDED
