#include <iostream>
#include "fasta.cpp"

using namespace std;

int main()
{
    /*
    Files:
    BigSequence
    NP_852610
    OctodonDegus_mRNA
    OctodonDegus_mRNA_noDescription
    WrongFormat_bigDescription
    WrongFormat_bigId
    WrongFormat_empty
    WrongFormat_wrongFirstSymbol

    */
    FASTA f1;
    f1.print();
    cout << endl;
    f1.open("../Samples/OctodonDegus_mRNA.fasta");
    f1.print();
    return 0;
}
