#include <iostream>
#include "fasta.cpp"

using namespace std;

int main()
{
    /*
    Files:
    240MB
    BigSequence
    NP_852610
    OctodonDegus_mRNA
    OctodonDegus_mRNA_noDescription
    WrongFormat_bigDescription
    WrongFormat_bigId
    WrongFormat_empty
    WrongFormat_wrongFirstSymbol
    UnknownSymbols
    */

    FASTA f1, f2;
    /*
    f1.read("../Samples/BigSequence.fasta");
    f1.print();
    f1.save("../Samples/Saved/BigSequence.fasta");
    */

    f1.setSequence("GACG");
    f1.print();
    f2.setSequence("ACAG");
    f2.print();
    f1.align(f2);

    return 0;
}
