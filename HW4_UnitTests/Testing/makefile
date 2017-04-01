main:
	g++ main.o mergesort.o comparion.o -o randomStrSorter
test_mergeSort: main.o mergeSort.o comparison.o
	g++ main.o mergesort.o comparion.o -o main_test
test_comparison: comparison.o
	g++ test_comparison.cpp -o comparison_test
test_strSwap: comparison.o
	g++ test_strSwap.cpp -o strSwap_test
main.o:
	g++ main.cpp -c
test_comparison.o: comparison.o
	g++ test_comparison.cpp -c
test_strSwap.o: comparison.o
	g++ test_strSwap.cpp -c
mergeSort.o: comparison.o
	g++ mergeSort.* -c
comparison.o:
	g++ comparison.* -c
clear:
	rm -f *.o *.h.gch *_test randomStrSorter