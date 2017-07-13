all:
	ar rc libheaan.a *.o
	g++ -L. -I/usr/local/include -I../src -std=c++11 HEAAN.cpp -lheaan -lntl -lgmp -lbsd -lpthread
	rm *.o
