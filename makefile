CC:=g++
CFLAGS:=-Wall -Wextra -Werror -pedantic -std=c++11 
INCLUDE:=-Iinclude
TEST_INCLUDE:=-Ilib/UnitTest++
DEBUG:=-ggdb -O0
PRODUCTION:=-g -O0
LIBS:=-lGLEW -lGLU -lGL -lX11
TEST_LIBS:=-Llib/UnitTest++ -lUnitTest++
TESTS:=$(wildcard tests/*.cpp)
TEST_OBJ:=$(patsubst tests/%.cpp, bin/tests/%.o, $(TESTS))
SOURCES:=$(wildcard src/*.cpp)
OBJECTS:=$(patsubst src/%.cpp, bin/%.o, $(SOURCES))
EXECUTABLE:=GLDemo
TEST_EXEC:=UnitTests

.PHONY: production
.PHONY: debug
.PHONY: tests
.PHONY: clean
.PHONY: valgrind
.PHONY: all

all: $(EXECUTABLE)

clean:
	rm -rf bin/*.o
	rm -rf bin/tests/*.o
	rm -f $(EXECUTABLE)
	rm -f $(TEST_EXEC)

debug: CFLAGS += $(DEBUG) $(INCLUDE)
debug: $(EXECUTABLE) tests

tests: CFLAGS += $(DEBUG) $(TEST_INCLUDE)
tests: $(TEST_EXEC)

production: CFLAGS += $(PRODUCTION) $(INCLUDE)
production: $(EXECUTABLE)

valgrind: debug
	valgrind --suppressions=res/custom.supp --leak-check=yes ./$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)

bin/%.o : src/%.cpp
	$(CC) -c $(CFLAGS) $< $(LIBS) -o $@

$(TEST_EXEC): $(TEST_OBJ)
	$(CC) $(TEST_OBJ) -o $@ $(TEST_LIBS)
	./$(TEST_EXEC)

bin/tests/%.o : tests/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@
