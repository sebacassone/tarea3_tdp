# Se crea un makefile para compilar tanto Simplex como test_Simplex
all: test_Simplex test_Branch

Simplex.o: Simplex.cpp Simplex.h
	g++ -c -g -O2 Simplex.cpp

Branch.o: Branch.cpp Branch.h
	g++ -c -g -O2 Branch.cpp

test_Simplex: test_Simplex.cpp Simplex.o
	g++ -O2 -g test_Simplex.cpp Simplex.o -o test_Simplex

test_Branch: test_Branch.cpp Branch.o Simplex.o
	g++ -O2 -g test_Branch.cpp Branch.o Simplex.o -o test_Branch

clean:
	rm -f *.o test_Simplex test_Branch

