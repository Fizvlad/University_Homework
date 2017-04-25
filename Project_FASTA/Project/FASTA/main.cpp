#include <iostream>
#include "fasta.cpp"

using namespace std;

int main()
{
    FASTA f1;
    f1.print();
    cout << endl;
    f1.open("../Samples/OctodonDegus_mRNA.fasta");
    f1.print();
    /*FILE* a = fopen("../Samples/OctodonDegus_mRNA.fasta", "rt");
    char* s = new char[256];
    fgets(s, 2, a);
    cout << s << "*" << endl;
    fgets(s, 256, a);
    int l = getStrLength(s);
    s[l - 1] = '\0';
    cout << s << "*" << endl;*/
    return 0;
}
