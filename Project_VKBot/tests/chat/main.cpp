#include <iostream>
#include <fstream>
#include "VK_API.h"

using namespace std;
using namespace vk_api;
using json = nlohmann::json;

int main()
{
    setlocale(LC_CTYPE, "rus");
    fstream fin("../../private/key.txt");
    string token;
    fin >> token;
    cout << "Token: " << token[0] << token[1] << token[2] << token[3] << "..." << endl;
    // Reading messages which were received while bot was offline
    return 0;
}
