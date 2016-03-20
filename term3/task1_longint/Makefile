all: prog
prog: longint.o main.o
	g++ longint.o main.o -o prog
longint.o: longint.h
	g++ -c longint.cpp
main.o: main.cpp longint.h
	g++ -c main.cpp
