#include "figures.h"
#include "list.h"
#include "bmp.h"

using namespace std;

typedef class scheme {
    inList list;
public:
    unsigned sizeX() {
        float out = 0;
        for (unsigned i = 0; i < list.getSize(); i++) {
            float temp = list[i]->getX() + list[i]->getSizeX();
            if (temp > out) {
                out = temp;
            }
        }
        return ceil(out);
    }
    unsigned sizeY() {
        float out = 0;
        for (unsigned i = 0; i < list.getSize(); i++) {
            float temp = list[i]->getY() + list[i]->getSizeY();
            if (temp > out) {
                out = temp;
            }
        }
        return ceil(out);
    }

    void print() {
        printf("%s%d%s%d%c", "Scheme size: ", sizeX(), ", ", sizeY(), '\n');
        for (unsigned i = 0; i < list.getSize(); i++) {
            printf("%d%s%s%c%s%s%c", i + 1, ". ", list[i]->print(), '\n', "Color: #", list[i]->getColor(), '\n');
        }
    }

    void draw(const char* Name) {
        BMP_Image img(sizeX(), sizeY(), Name);
        for (unsigned i = 0; i < list.getSize(); i++) {
            list[i]->draw(&img);
        }
        img.render();
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
