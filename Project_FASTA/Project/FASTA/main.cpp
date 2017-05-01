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
    f1.save("../Samples/Saved/Empty.fasta");
    f1.setId("MySequence");
    f1.setDescription("Description");
    f1.setSequence("ABCDEFG");
    f1.print();
    f1.save("../Samples/Saved/MySequence.fasta");

    FASTA f2;
    f2.read("../Samples/Saved/MySequence.fasta");
    f2.print();
    return 0;
}
