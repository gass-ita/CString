all: test
	./test

cstring.o: cstring.cc cstring.h
	g++ -c cstring.cc -o cstring.o

test: cstring.o test.cc
	g++ cstring.o test.cc -o test