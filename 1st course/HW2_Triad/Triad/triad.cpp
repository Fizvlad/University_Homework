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
    const triad& operator +=(const triad& in) {
        for (int i = 0; i < 3; i++) {
            this->num[i] += in.num[i];
        }
        return *this;
    }
    const triad& operator -=(const triad& in) {
        for (int i = 0; i < 3; i++) {
            this->num[i] -= in.num[i];
        }
        return *this;
    }
    const triad& operator *=(const int& in) {
        for (int i = 0; i < 3; i++) {
            this->num[i] *= in;
        }
        return *this;
    }
    // Constructor/Destructor
    triad(int in0 = 0, int in1 = 0, int in2 = 0) {
        num = new int[3];
        num[0] = in0;
        num[1] = in1;
        num[2] = in2;
    }
    triad(const triad& in) {
        num = new int[3];
        for (int i = 0; i < 3; i++) {
            this->num[i] = in.num[i];
        }
    }
    triad& operator = (const triad& in) {
        for (int i = 0; i < 3; i++) {
            this->num[i] = in.num[i];
        }
        return *this;
    }
    ~triad() {
        delete num;
    }
};

const triad& operator + (triad in1, const triad& in2) {
    return in1 += in2;
}
const triad& operator - (triad in1, const triad& in2) {
    return in1 -= in2;
}
const triad& operator * (triad in, const int& i) {
    return in *= i;
}
const triad& operator * (const int& i, triad in) {
    return in *= i;
}
