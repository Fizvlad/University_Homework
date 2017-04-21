#include <iostream>
#include "fasta.cpp"

using namespace std;

int main()
{
    FILE* a = fopen("../Samples/OctodonDegus_mRNA.fasta", "rt");
    cout << (unsigned char) getc(a) << endl;
    char* s = new char[128];
    fscanf(a, "%s", s);
    cout << s << endl;
    getc(a); // skipping space
    fgets(s, 128, a);
    cout << s << endl;
    unsigned l = 0;
    int c;
    c = getc(a);
    while (c > 0) {
        if ((c >= 65 && c <= 90) || c == 42 || c == 45) {
            cout << (unsigned char)c;
            l++;
        }
        c = getc(a);
    }
    cout << endl << l << endl;
    return 0;
}
