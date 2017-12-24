#include <string>
#include <iostream>
#include <fstream>
#include <clocale>

#include "nlohmann/json.hpp" // gcc 4.2+ required

#include "VK_API.h"

using json = nlohmann::json;
using namespace vk_api;
using namespace std;

int main()
{
    setlocale(LC_CTYPE, "rus");
    fstream fin("../../private/key.txt");
    fstream fout("messages.txt", fstream::app);
    string token;
    fin >> token;
    cout << "Token: " << token[0] << token[1] << token[2] << token[3] << "..." << endl;

    longpoll::session s;
    s.initialize(token);
    s.listen([&](json upd){
                if (upd.size() != 0)
                    cout << "Received new events." << endl;
                else
                    cout << "No new events." << endl;
                for (auto i : upd) {
                    cout << "  " << i << endl;
                    if (i[0] == longpoll::EVENTS::MESSAGE::NEW)
                        fout << "User with id" << i[3] << " at " << i[4] << ": " << i[5] << endl;
                    if (i[5] == "!stop")
                        return false;
                }
                return true;
             });
    return 0;
}
