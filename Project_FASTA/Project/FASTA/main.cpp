#include <iostream>
#include "fasta.cpp"

using namespace std;

int main()
{
    FASTA f1;
    f1.print();
    f1.open("../Samples/OctodonDegus_mRNA_noDescription.fasta");
    f1.print();
    return 0;
}
