#include "figures.h"
#include "list.h"

using namespace std;

typedef class scheme {
    inList list;
public:
    void print() {
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
