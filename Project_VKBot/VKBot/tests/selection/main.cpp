#include <iostream>
#include <fstream>

#include "selection/VK_selection.h"

using namespace std;
using namespace vk_selection;

int main()
{
    fstream fin1("private/key.txt");
    string token;
    fin1 >> token;
    cout << "Token: " << token[0] << token[1] << token[2] << token[3] << "..." << endl;

    fstream fin2("private/serviceKey.txt");
    string service_token;
    fin2 >> service_token;
    cout << "Service token: " << service_token[0] << service_token[1] << service_token[2] << service_token[3] << "..." << endl;

    Unit habr = Unit("habr");
    Unit gag = Unit("ru9gag");
    Unit kinopoisk = Unit("kinopoisk");
    Unit stepik = Unit("rustepik");
    Unit Filyus_Bot = Unit(283177744, true);
    Unit Fizvlad = Unit("id157230821");
    Unit buerak = Unit("buerak_spring");
    cout << habr << endl << gag << endl << kinopoisk << endl << stepik << endl << Filyus_Bot << endl << Fizvlad << endl << buerak << endl;

    // Works fine
    Selection test = Filyus_Bot.friends();
    test.saveAs("Filyus_friends");

    /*
    // Requires personal accessToken
    Selection test2 = Fizvlad.subscribers(service_token);
    test2.saveAs("test2");
    */

    // Works fine. Requires much time
    Selection test3 = kinopoisk.members(token);
    test3.saveAs("Kinopoisk_subscribers");
    return 0;
}
