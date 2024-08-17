# Se crea un makefile para compilar tanto Simplex como test_Simplex
all: test_Simplex test_Branch main

Simplex.o: Simplex.cpp Simplex.h
	g++ -c -g -O2 Simplex.cpp

Branch.o: Branch.cpp Branch.h Compare.h Element.h
	g++ -c -g -O2 Branch.cpp

test_Simplex: test_Simplex.cpp Simplex.o
	g++ -O2 -g test_Simplex.cpp Simplex.o -o test_Simplex

test_Branch: test_Branch.cpp Branch.o Simplex.o
	g++ -O2 -g test_Branch.cpp Branch.o Simplex.o -o test_Branch

main: main.cpp Simplex.o Branch.o
	g++ -O2 -g main.cpp Simplex.o Branch.o -o main

clean:
	rm -f *.o test_Simplex test_Branch main
