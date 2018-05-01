#include <iostream>
#include <fstream>

#include "selection/VK_selection.h"

using namespace std;
using namespace vk_selection;

int main()
{
    fstream fin("private/key.txt");
    string token;
    fin >> token;
    cout << "Token: " << token[0] << token[1] << token[2] << token[3] << "..." << endl;

    Unit habr = Unit("habr");
    Unit gag = Unit("ru9gag");
    Unit stepik = Unit("rustepik");
    Unit Filyus_Bot = Unit(283177744, true);
    Unit buerak = Unit("buerak_spring");
    cout << habr << endl << gag << endl << stepik << endl << Filyus_Bot << endl << buerak << endl;

    Selection test = Filyus_Bot.friends();
    test.saveAs("bot_friends");
    return 0;
}
