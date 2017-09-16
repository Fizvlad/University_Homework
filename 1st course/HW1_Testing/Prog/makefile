MyProg: longNumbers.o createNumber.o
	g++ longNumbers.o createNumber.o main.cpp -o MyProg
createNumber.o:
	g++ createNumber.h createNumber.cpp -c
longNumbers.o:
	g++ longNumbers.h longNumbers.cpp -c
clean:
	rm -f *.o *.h.gch MyProg
