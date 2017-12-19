#include <iostream>
#include <fstream>
#include <string>

#include "nlohmann/json.hpp" // gcc 4.2+ required

#include "VK_API.h"

using json = nlohmann::json;
using namespace vk_api;
using namespace std;

int main()
{
    fstream fin("../../private/key.txt");
    string token;
    fin >> token;
    cout << "Token: " << token[0] << token[1] << token[2] << token[3] << "..." << endl;

    return 0;
}
