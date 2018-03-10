all: test.o
	g++ test.o -o test

test.o:
	g++ -Wall -c test.cpp

clean:
	rm test.o test
