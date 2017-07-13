all:
	ar rc libheac.a *.o
	g++ -L. -I/usr/local/include -I../src -std=c++11 HEAC.cpp -lheac -lntl -lgmp -lbsd -lpthread
	rm *.o
