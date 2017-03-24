#include <iostream>
#include "scheme.h"

using namespace std;

int main()
{
    point a(1, 1.337);
    point b(2, 4);
    point c(18, 18);
    point d(5.5, 5);
    cut cu;
    cu.set(a, b);
    cu.setColor(123);
    triangle t;
    t.set(a, b, c);
    qadrangle q;
    q.set(a, b, c, d);
    rectangle r;
    r.set(a, b, 3.33);
    square s;
    s.set(a, b);
    circle ci;
    ci.set(d, 13);

    scheme s1;
    s1.push(&t);
    s1.push(&q);
    s1.push(&r);
    s1.push(&s);
    s1.push(&ci);
    s1.push(&cu);
    s1.push(&c);
    s1.print();
    return 0;
}
