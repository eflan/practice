all: Stocks

Stocks: Stocks.o
	g++ -Wall -std=c++11 Stocks.o -o Stocks

Stocks.o: Stocks.cpp stdafx.h
	g++ -Wall -std=c++11 -c Stocks.cpp -o Stocks.o
