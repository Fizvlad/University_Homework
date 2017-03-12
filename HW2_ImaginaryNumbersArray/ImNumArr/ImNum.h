#ifndef IMNUM_H_INCLUDED
#define IMNUM_H_INCLUDED

#include "ImNum.cpp"

typedef class imNumber;

const imNumber& operator+ (imNumber a, const imNumber& b);
const imNumber& operator- (imNumber a, const imNumber& b);
const imNumber& operator* (imNumber a, const imNumber& b);
const imNumber& operator* (imNumber a, const float& b);
const imNumber& operator* (const float& a, imNumber b);

bool operator== (imNumber& a, imNumber& b);
bool operator!= (imNumber& a, imNumber& b);

float abs(imNumber& in);


#endif // IMNUM_H_INCLUDED
