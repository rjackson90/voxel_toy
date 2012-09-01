CC:=g++
CFLAGS:=-Wall -Wextra -Werror -pedantic -std=c++0x -Iinclude
DEBUG:=-g -pg -O0
PRODUCTION:=-O2
LIBS:=-lGL -lX11
SOURCES:=$(wildcard src/*.cpp)
OBJECTS:=$(patsubst src/%.cpp, bin/%.o, $(SOURCES))
EXECUTABLE:=GLDemo

.PHONY: production
.PHONY: debug
.PHONY: clean
.PHONY: valgrind
.PHONY: all

all: $(EXECUTABLE)

clean:
	rm -rf bin/*
	rm -f $(EXECUTABLE)

debug: CFLAGS += $(DEBUG)
debug: $(EXECUTABLE)

production: CFLAGS += $(PRODUCTION)
production: $(EXECUTABLE)

valgrind: clean debug
	valgrind --suppressions=res/custom.supp --leak-check=yes ./$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@ 

bin/%.o : src/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@ 
	
