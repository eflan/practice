CC=g++
CFLAGS=-Wall -std=c++11 -c
LFLAGS=-Wall -std=c++11
SOURCES=BlackJack.cpp CallCenter.cpp ObjectOrientedDesign.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ObjectOrientedDesign

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)

clean:
	rm *.o ObjectOrientedDesign
