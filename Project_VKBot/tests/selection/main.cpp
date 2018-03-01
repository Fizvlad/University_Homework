#include <iostream>

#include "selection/VK_selection.h"

using namespace std;
using namespace vk_selection;

int main()
{
    Unit habr = Unit("habr");
    Unit gag = Unit("ru9gag");
    Unit stepik = Unit("rustepik");
    Unit durov = Unit(1, true);
    cout << habr << endl << gag << endl << stepik << endl << durov << endl;
    return 0;
}
