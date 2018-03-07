#include <iostream>

#include "selection/VK_selection.h"

using namespace std;
using namespace vk_selection;

int main()
{
    Unit habr = Unit("habr");
    Unit gag = Unit("ru9gag");
    Unit stepik = Unit("rustepik");
    Unit pasha = Unit(1, true);
    Unit buerak = Unit("buerak_spring");
    cout << habr << endl << gag << endl << stepik << endl << pasha << endl << buerak << endl;
    Selection a("test");
    return 0;
}
