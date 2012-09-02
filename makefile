CC:=g++
CFLAGS:=-Wall -Wextra -Werror -pedantic -std=c++0x -Iinclude
DEBUG:=-g -O0
PRODUCTION:=-g -O0
LIBS:=-lGL -lX11 -lrt
SOURCES:=src/glwindow.cpp src/main.cpp
OBJECTS:=$(patsubst src/%.cpp, bin/%.o, $(SOURCES))
EXECUTABLE:=GLDemo

.PHONY: production
.PHONY: debug
.PHONY: clean
.PHONY: valgrind
.PHONY: all

all: clean $(EXECUTABLE)

clean:
	rm -rf bin/*
	rm -f $(EXECUTABLE)

debug: CFLAGS += $(DEBUG)
debug: clean $(EXECUTABLE)

production: CFLAGS += $(PRODUCTION)
production: clean $(EXECUTABLE)

valgrind: clean debug
	valgrind --suppressions=res/custom.supp --leak-check=yes ./$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) bin/instrumentation.o
	$(CC) $(OBJECTS) bin/instrumentation.o $(LIBS) -o $@ 

bin/%.o : src/%.cpp
	$(CC) -c $(CFLAGS) -finstrument-functions $< -o $@

bin/instrumentation.o : src/instrumentation.cpp
	$(CC) -c $(CFLAGS) $< -o $@
	
