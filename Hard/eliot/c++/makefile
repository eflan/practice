CC=g++
CFLAGS=-Wall -std=c++11
LFLAGS=-Wall -std=c++11
SOURCES=main.cpp wordDistance.cpp longestWord.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Hard

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm $(OBJECTS) $(EXECUTABLE)
