#include <cmath>
#include <iostream>

using namespace std;

typedef class imNumber {
    float Re;
    float Im;
public:
    void set(float r, float i) {
        Re = r;
        Im = i;
    }
    float getReal() {
        return Re;
    }
    float getImaginary() {
        return Im;
    }

    float abs() {
        return sqrt(Re * Re + Im * Im);
    }

    const imNumber& operator+= (const imNumber& in) {
        Re += in.Re;
        Im += in.Im;
        return *this;
    }
    const imNumber& operator-= (const imNumber& in) {
        Re -= in.Re;
        Im -= in.Im;
        return *this;
    }
    const imNumber& operator*= (const imNumber in) { // we do not use (const imNumber& in) to prevent error on a *= a
        float temp = Re;
        Re = Re * in.Re - Im * in.Im;
        Im = Im * in.Re + temp * in.Im;
        return *this;
    }
    const imNumber& operator*= (const float& in) {
        Re *= in;
        Im *= in;
        return *this;
    }

    imNumber() {
        Re = 0;
        Im = 0;
    }
    imNumber(float a, float b = 0) {
        Re = a;
        Im = b;
    }
    imNumber(const imNumber& in) {
        Re = in.Re;
        Im = in.Im;
    }
    ~imNumber() {
    }
};

const imNumber& operator+ (imNumber a, const imNumber& b) {
    return a += b;
}
const imNumber& operator- (imNumber a, const imNumber& b) {
    return a -= b;
}
const imNumber& operator* (imNumber a, const imNumber& b) {
    return a *= b;
}
const imNumber& operator* (imNumber a, const float& b) {
    return a *= b;
}
const imNumber& operator* (const float& a, imNumber b) {
    return b *= a;
}

bool operator== (imNumber& a, imNumber& b) {
    return (a.getReal() == b.getReal()) && (a.getImaginary() == b.getImaginary());
}
bool operator!= (imNumber& a, imNumber& b) {
    return !(a == b);
}

float abs(imNumber& in) {
    return in.abs();
}
