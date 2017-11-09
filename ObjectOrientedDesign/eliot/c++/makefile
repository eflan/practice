CC=g++
CFLAGS=-Wall -std=c++11
LFLAGS=-Wall -std=c++11
SOURCES=BlackJack.cpp CallCenter.cpp ObjectOrientedDesign.cpp ParkingLot.cpp ereader.cpp hashtable.cpp jigsaw.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ObjectOrientedDesign

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm $(OBJECTS) $(EXECUTABLE)
