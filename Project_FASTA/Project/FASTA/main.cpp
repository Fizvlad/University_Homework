#include <iostream>
#include "fasta.cpp"

using namespace std;

int main()
{
    /*
    "../Samples/__________.fasta"
    Files:
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
    f1.setSequence("ACGTCAGACGACTACTACGATCGACGACTATCTATATATCTCGAGCTACGACTTACGAGCATCAT");
    cout << endl << endl;
    f2.setSequence("ATCGTATCGCGCTATAGGGCGGCAGACACACACGGTATGGACAGAGGCAGT");
    cout << endl << endl;
    f1.align(f2);

    /*
    FASTA f1;
    f1.read("../Samples/OctodonDegus_mRNA.fasta");
    f1.print();
    f1.save("../Samples/Saved/OctodonDegus_mRNA.fasta");
    FASTA f2;
    f2.read("../Samples/Saved/OctodonDegus_mRNA.fasta");
    f2.print();
    */
    return 0;
}
