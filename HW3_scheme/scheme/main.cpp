#include <iostream>
#include "scheme.h"

using namespace std;

int main()
{
    scheme s;
    cut cu(point(20,10), point(33, 37));
    cu.setColor("aaaaaa");
    s.push(&cu);

    triangle tr(point(60, 50), point(100, 9.111), point(70, 10));
    tr.setColor("0000ff");
    s.push(&tr);

    qadrangle qa(point(50, 300), point(70, 320), point(100, 150), point(10, 10));
    s.push(&qa);

    rectangle re(point(20, 189), point(25, 190), 50.37);
    re.setColor("ff00aa");
    s.push(&re);

    square sq(point (10, 200), point(18, 210));
    s.push(&sq);

    circle ci(point(60, 150), 25);
    s.push(&ci);

    s.print();
    s.draw("output.bmp");
    return 0;
}
