#include <iostream>
#include <string>
#include <fstream>
#include <regex>

using namespace std;

int main()
{
    ifstream fin("input.txt");
    ofstream fout("output.txt");
    regex re(R"(^(https?:\/\/)?([0-9a-z-]+\.)+([a-z]{2,6})(\/[\w \.-]+)*\/?(\?[\w_]+=[\w_]+(&[\w_]+=[\w_]+)*)?(#[\w_]*)?$)");
    string str;
    while (fin >> str) {
        if (regex_match(str, re)) {
            fout << str << endl;
        }
    }
    return 0;
}
