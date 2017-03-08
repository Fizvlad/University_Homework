#include <iostream>

using namespace std;

typedef class triad {
    int* num;
public:
    // Set/get functions
    int getNum(unsigned pos) { //Position from 0 to 2
        return num[pos];
    }
    void set(int in0 = 0, int in1 = 0, int in2 = 0) { // Fully set triad
        num[0] = in0;
        num[1] = in1;
        num[2] = in2;
    }
    void setNum(int in, unsigned pos) { //Position from 0 to 2
        num[pos] = in;
    }
    // Operators
    triad& operator +=(triad& in) {
        for (int i = 0; i < 3; i++) {
            this->num[i] += in.getNum(i);
        }
        return *this;
    }
    triad& operator -=(triad& in) {
        for (int i = 0; i < 3; i++) {
            this->num[i] -= in.getNum(i);
        }
        return *this;
    }

    triad& operator + (triad& in) {
        triad* out = new triad;
        for (int i = 0; i < 3; i++) {
            out->num[i] = num[i] + in.getNum(i);
        }
        return *out;
    }
    //Constructor/Destructor
    triad(int in0 = 0, int in1 = 0, int in2 = 0) {
        num = new int[3];
        num[0] = in0;
        num[1] = in1;
        num[2] = in2;
    }
    triad(triad& in) {
        num = new int[3];
        for (int i = 0; i < 3; i++) {
            this->num[i] = in.getNum(i);
        }
    }
    triad& operator = (triad& in) {
        for (int i = 0; i < 3; i++) {
            this->num[i] = in.getNum(i);
        }
        return *this;
    }
    ~triad() {
        delete num;
    }
};
