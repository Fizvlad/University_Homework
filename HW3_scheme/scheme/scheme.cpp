#include "figures.h"
#include "list.h"

using namespace std;

typedef class scheme {
    inList list;
public:
    float sizeX() {
        float out = 0;
        for (unsigned i = 0; i < list.getSize(); i++) {
            float temp = list[i]->getX() + list[i]->getSizeX();
            if (temp > out) {
                out = temp;
            }
        }
        return out;
    }
    float sizeY() {
        float out = 0;
        for (unsigned i = 0; i < list.getSize(); i++) {
            float temp = list[i]->getY() + list[i]->getSizeY();
            if (temp > out) {
                out = temp;
            }
        }
        return out;
    }

    void print() {
        printf("%s%.3f%s%.3f%c", "Scheme size: ", sizeX(), ", ", sizeY(), '\n');
        for (unsigned i = 0; i < list.getSize(); i++) {
            printf("%d%s%s%c%s%d%c", i + 1, ". ", list[i]->print(), '\n', "Color: ", list[i]->getColor(), '\n');
        }
    }

    void push(Figure* in) {
        list.push(in);
    }
    Figure* pop() {
        return list.pop();
    }

    scheme() {
    }
    scheme(const scheme& in) {
        list = in.list;
    }
    ~scheme() {
    }
};
