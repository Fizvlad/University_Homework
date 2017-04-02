strSorter: main.o
	g++ main.o -o strSorter
test_mergeSort: main.o
	g++ main.o -o Testing/test_mergeSort/test_mergeSort
	./bash_test test_mergeSort
test_comparison: test_comparison.o
	g++ test_comparison.o -o Testing/test_comparison/test_comparison
	./bash_test test_comparison
test_strSwap: test_strSwap.o
	g++ test_strSwap.o -o Testing/test_strSwap/test_strSwap
	./bash_test test_strSwap
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
	rm -f *.o *.h.gch strSorter
