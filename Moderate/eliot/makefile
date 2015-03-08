CC=g++
CFLAGS=-Wall -std=c++11
LFLAGS=-Wall -std=c++11
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Moderate

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm $(OBJECTS) $(EXECUTABLE)
