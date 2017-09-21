all: dedupURLs

dedupURLs: dedupURLs.o
	g++ -Wall -std=c++11 dedupURLs.o -o dedupURLs

dedupURLs.o: dedupURLs.cpp
	g++ -Wall -std=c++11 -c dedupURLs.cpp -o dedupURLs.o
