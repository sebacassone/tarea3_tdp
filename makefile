# Se crea un makefile para compilar tanto Simplex como test_Simplex
all: test_Simplex

Simplex.o: Simplex.cpp Simplex.h
	g++ -c -g -O2 Simplex.cpp

test_Simplex: test_Simplex.cpp Simplex.o
	g++ -O2 -g test_Simplex.cpp Simplex.o -o test_Simplex

clean:
	rm -f *.o test_Simplex

