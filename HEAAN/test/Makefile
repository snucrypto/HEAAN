all:
	ar rc libheaan.a ../src/*.o
	g++ -L. -I/usr/local/include -I../src -std=c++11 HEAAN.cpp -lheaan -lntl -lgmp -pthread
	
clean:
	rm ../src/*.o
