#include <iostream>
#include "fasta.h"

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
    f1.setId("Example1");
    f1.setSequence("ACGTCAGACGACTACTACGATCGACGACTATCTATATATCTCGAGCTACGACTTACGAGCATCAT");
    f1.print();
    cout << endl << endl;
    f2.setId("Example2");
    f2.setSequence("ATCGTATCGCGCTATAGGGCGGCAGACACACACGGTATGGACAGAGGCAGT");
    f2.print();
    cout << endl << endl;
    f1.align(f2);
    cout << endl << endl;





    f1.setSequence("AAAGGGCCC-");
    f1.print();
    f1.printStat();
    /*
    f1.save("../Samples/Saved/OctodonDegus_mRNA.fasta");
    f2.read("../Samples/Saved/OctodonDegus_mRNA.fasta");
    f2.print();
    */

    return 0;
}
