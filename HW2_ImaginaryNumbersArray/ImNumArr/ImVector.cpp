#include "imNum.h"

using namespace std;

typedef class imVector {
    unsigned length;
    imNumber* arr;

    friend void swap(imVector& a, imVector& b) {
        unsigned tl = b.length;
        imNumber* ta = b.arr;
        b.length = a.length;
        b.arr = a. arr;
        a.length = tl;
        a.arr = ta;
    }
public:
    unsigned getLength() {
        return length;
    }
    void resize(unsigned newLength) {
        imNumber* temp = new imNumber[newLength];
        for (unsigned i = 0; i < newLength && i < length; i++) {
            temp[i] = arr[i];
        }
        length = newLength;
        delete arr;
        arr = temp;
    }
    void push_back(imNumber in) {
        resize(length + 1);
        arr[length - 1] = in;
    }

    imNumber& operator[](unsigned p) {
        return arr[p];
    }

    imVector(unsigned l) {
        length = l;
        arr = new imNumber[length];
    }
    ~imVector() {
        delete arr;
    }
    imVector(const imVector& in) {
        length = in.length;
        arr = new imNumber[length];
        for (unsigned i =0; i < length; i++) {
            arr[i] = in.arr[i];
        }
    }
    imVector& operator= (const imVector& in) {
        imVector temp(in);
        swap(*this, temp);
        return *this;
    }
    imVector& operator= (imVector&& in) {
        swap(*this, in);
        return *this;
    }
};

bool operator== (imVector& a, imVector& b) {
    if (a.getLength() != b.getLength()) {
        return false;
    } else {
        for (unsigned i = 0; i < a.getLength(); i++) {
            if (a[i] != b[i]) {
                return false;
            }
        }
    }
    return true;
}
bool operator!= (imVector& a, imVector& b) {
    return !(a == b);
}
