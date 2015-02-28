all: ObjectOrientedDesign

ObjectOrientedDesign: ObjectOrientedDesign.o BlackJack.o
	g++ -Wall -std=c++11 ObjectOrientedDesign.o BlackJack.o -o ObjectOrientedDesign

BlackJack.o: BlackJack.cpp
	g++ -Wall -std=c++11 -c BlackJack.cpp -o BlackJack.o

ObjectOrientedDesign.o: ObjectOrientedDesign.cpp
	g++ -Wall -std=c++11 -c ObjectOrientedDesign.cpp -o ObjectOrientedDesign.o
