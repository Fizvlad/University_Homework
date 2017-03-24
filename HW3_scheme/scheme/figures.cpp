#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "utility.h"

using namespace std;

typedef class Figure {
protected:
    float X;
    float Y;
    float sizeX;
    float sizeY;
    short color; // R(0-7)G(0-7)B(0-7)
public:
    short getColor() {
        return color;
    }
    void setColor(short in) {
        color = in;
    }
    float getX() {
        return X;
    }
    float getY() {
        return Y;
    }
    float getSizeX() {
        return sizeX;
    }
    float getSizeY() {
        return sizeY;
    }

    virtual char* print() {
        return "Base type print()";
    };

    Figure() {
        X = -1;
        Y = -1;
        sizeX = 0;
        sizeY = 0;
        color = 000;
    }
    Figure(const Figure& in) {
        X = in.X;
        Y = in.Y;
        sizeX = in.sizeX;
        sizeY = in.sizeY;
        color = in.color;
    }
    ~Figure() {
    }
};

typedef class point : public Figure {
    /*
    This class have +, * operators, so it can be used as vector
    X and Y are equal to Figure's one
    sizeX, sizeY constantly 1
    */
public:
    float x() {
        return X;
    }
    float y() {
        return Y;
    }
    void set(float x, float y) {
        X = x;
        Y = y;
    }
    float distance(const point& in) {
        return sqrt(pow(X - in.X, 2) + pow(Y - in.Y, 2));
    }

    char* print() {
        int MAX_LENGTH = 32;
        char* temp = new char[MAX_LENGTH];
        int l;
        l = sprintf(temp, "%c%.3f%s%.3f%c",'(' , X, "; ", Y, ')');
        if (l == EOF || l > MAX_LENGTH) {
            exit(EXIT_FAILURE);
        }
        char* out = new char[l + 1];
        out[l] = '\0';
        for (int i = 0; i < l; i++) {
            out[i] = temp[i];
        }
        delete temp;
        return out;
    }

    point& ortogonalize() {
        float t = X;
        X = Y;
        Y = -1 * t;
        return *this;
    }

    point& operator += (const point& in) {
        X += in.X;
        Y += in.Y;
        return *this;
    }
    point& operator -= (const point& in) {
        X -= in.X;
        Y -= in.Y;
        return *this;
    }
    point& operator *= (const float& in) {
        X *= in;
        Y *= in;
        return *this;
    }

    point(float x = -1, float y = -1) {
        X = x;
        Y = y;
        sizeX = 1;
        sizeY = 1;
    }
    point(const point& in) {
        X = in.X;
        Y = in.Y;
        sizeX = 1;
        sizeY = 1;
    }
    ~point() {
    }
};

const point& operator+ (point a, const point& b) {
    return a += b;
}
const point& operator- (point a, const point& b) {
    return a -= b;
}
float operator* (point& a, point& b) {
    return a.x() * b.x() + a.y() * b.y();
}
const point& operator* (point a, const float& b) {
    return a *= b;
}
const point& operator* (const float& a, point b) {
    return b *= a;
}

typedef class cut : public Figure {
    point Begin;
    point End;
public:
    point begin() {
        return Begin;
    }
    point end() {
        return End;
    }
    void set(point b, point e) {
        Begin = b;
        End = e;
        crop();
    }

    void crop() {
        X = min(Begin.x(), End.x());
        Y = min(Begin.y(), End.y());
        sizeX = max(Begin.x(), End.x()) - X;
        sizeY = max(Begin.y(), End.y()) - Y;
    }

    char* print() {
        char* out = merge(4, "Cut:\n", Begin.print(), " ", End.print());
        return out;
    }

    cut() {
    }
    cut(point b, point e) {
        Begin = b;
        End = e;
        crop();
    }
    cut(const cut& in) {
        Begin = in.Begin;
        End = in.End;
    }
    ~cut() {
    }
};

typedef class triangle : public Figure {
    point pA;
    point pB;
    point pC;
public:
    point A() {
        return pA;
    }
    point B() {
        return pB;
    }
    point C() {
        return pC;
    }
    void set(point a, point b, point c) {
        pA = a;
        pB = b;
        pC = c;
        crop();
    }
    void crop() {
        X = min(pA.x(), pB.x(), pC.x());
        Y = min(pA.y(), pB.y(), pC.y());
        sizeX = max(pA.x(), pB.x(), pC.x()) - X;
        sizeY = max(pA.y(), pB.y(), pC.y()) - Y;
    }

     char* print() {
        char* out = merge(6, "Triangle:\nA: ", pA.print(), "\nB: ", pB.print(), "\nC: ", pC.print());
        return out;
    }

    triangle() {
        pA.set(-1, -1);
        pB.set(-1, -1);
        pC.set(-1, -1);
    }
    triangle(const triangle& in) {
        pA = in.pA;
        pB = in.pB;
        pC = in.pC;
    }
    ~triangle() {
    }
};

typedef class qadrangle : public Figure {
    point pA;
    point pB;
    point pC;
    point pD;
public:
    point A() {
        return pA;
    }
    point B() {
        return pB;
    }
    point C() {
        return pC;
    }
    point D() {
        return pD;
    }
    void set(point a, point b, point c, point d) {
        pA = a;
        pB = b;
        pC = c;
        pD = d;
        crop();
    }
    void crop() {
        X = min(pA.x(), pB.x(), pC.x(), pD.x());
        Y = min(pA.y(), pB.y(), pC.y(), pD.x());
        sizeX = max(pA.x(), pB.x(), pC.x(), pD.x()) - X;
        sizeY = max(pA.y(), pB.y(), pC.y(), pD.y()) - Y;
    }

     char* print() {
        char* out = merge(8, "Qadrangle:\nA: ", pA.print(), "\nB: ", pB.print(), "\nC: ", pC.print(), "\nD: ", pD.print());
    }

    qadrangle() {
        pA.set(-1, -1);
        pB.set(-1, -1);
        pC.set(-1, -1);
        pD.set(-1, -1);
    }
    qadrangle(const qadrangle& in) {
        pA = in.pA;
        pB = in.pB;
        pC = in.pC;
        pD = in.pD;
    }
    ~qadrangle() {
    }
};

typedef class rectangle : public Figure {
    point pA;
    point pB;
    float Length;
public:
    point A() {
        return pA;
    }
    point B() {
        return pB;
    }
    point C() {
        point pC;
        if (pB.distance(pA) == 0) {
            pC = pA;
            return pC;
        }
        float k = Length / pB.distance(pA);
        point t = (pB - pA) * k;
        t.ortogonalize();
        pC = pB + t;
        return pC;
    }
    point D() {
        point pD;
        if (pB.distance(pA) == 0) {
            pD = pA;
            return pD;
        }
        float k = Length / pB.distance(pA);
        point t = (pB - pA) * k;
        t.ortogonalize();
        pD = pA + t;
        return pD;
    }
    float length() {
        return Length;
    }
    float height() {
        return pB.distance(pA);
    }
    void set(point a, point b, float l) {
        pA = a;
        pB = b;
        Length = l;
        crop();
    }
    void crop() {
        point pC = C();
        point pD = D();
        X = min(pA.x(), pB.x(), pC.x(), pD.x());
        Y = min(pA.y(), pB.y(), pC.y(), pD.x());
        sizeX = max(pA.x(), pB.x(), pC.x(), pD.x()) - X;
        sizeY = max(pA.y(), pB.y(), pC.y(), pD.y()) - Y;
    }

     char* print() {
        char* out = merge(8, "Rectangle:\nA: ", pA.print(), "\nB: ", pB.print(), "\nC: ", C().print(), "\nD: ", D().print());
        return out;
    }

    rectangle() {
        pA.set(-1, -1);
        pB.set(-1, -1);
        Length = 0;
    }
    rectangle(const rectangle& in) {
        pA = in.pA;
        pB = in.pB;
        Length = in.Length;
    }
    ~rectangle() {
    }
};

typedef class square : public Figure {
    point pA;
    point pC;
public:
    point A() {
        return pA;
    }
    point B() {
        point pB;
        if (pC.distance(pA) == 0) {
            pB = pA;
            return pB;
        }
        point t = (pC - pA) * 0.5;
        t.ortogonalize();
        pB = (pA + pC) * 0.5 + t;
        return pB;
    }
    point C() {
        return pC;
    }
    point D() {
        point pD;
        if (pC.distance(pA) == 0) {
            pD = pA;
            return pD;
        }
        point t = (pC - pA) * 0.5;
        t.ortogonalize();
        pD = (pA + pC) * 0.5 - t;
        return pD;
    }
    void set(point a, point c) {
        pA = a;
        pC = c;
        crop();
    }
    void crop() {
        point pB = B();
        point pD = D();

        X = min(pA.x(), pB.x(), pC.x(), pD.x());
        Y = min(pA.y(), pB.y(), pC.y(), pD.x());
        sizeX = max(pA.x(), pB.x(), pC.x(), pD.x()) - X;
        sizeY = max(pA.y(), pB.y(), pC.y(), pD.y()) - Y;
    }

     char* print() {
        char* out = merge(8, "Square:\nA: ", pA.print(), "\nB: ", B().print(), "\nC: ", pC.print(), "\nD: ", D().print());
        return out;
    }

    square() {
        pA.set(-1, -1);
        pC.set(-1, -1);
    }
    square(const square& in) {
        pA = in.pA;
        pC = in.pC;
    }
    ~square() {
    }
};

typedef class circle : public Figure {
    point pO;
    float R;
public:
    point O() {
        return pO;
    }
    float r() {
        return R;
    }
    void set(point o, float r) {
        pO = o;
        R = r;
        crop();
    }
    void crop() {
        X = pO.x() - R;
        Y = pO.y() - R;
        sizeX = 2 * R;
        sizeY = 2 * R;
    }

     char* print() {
        char* out = merge(4, "Circle:\nCenter: ", pO.print(), "\nRadius: ", floatToString(R));
        return out;
    }

    circle() {
        pO.set(-1, -1);
        R = 0;
    }
    circle(const circle& in) {
        pO = in.pO;
        R = in.R;
    }
    ~circle() {
    }
};

