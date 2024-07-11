CC = g++
INCLUDE = -Iheaders
FLAGS = $(INCLUDE) -Wall -pedantic -g --std=c++20

BIN = ./catan
TESTBIN = ./catan-test

TESTS = $(wildcard src/test/*.cpp)
SRC = $(wildcard src/*.cpp) $(wildcard src/ui/*.cpp)

OBJ = $(SRC:src/%.cpp=obj/%.o)
TEST_OBJ = $(OBJ:obj/main.o=) $(TESTS:src/%.cpp=obj/%.o)
DEP = $(SRC:src/%.cpp=dep/%.d) $(TESTS:src/%.cpp=dep/%.d)

.PHONY: compile clean init test

compile: init $(BIN) test

clean:
	rm -rf $(BIN) obj/* dep/*


init:
	mkdir -p obj obj/ui obj/test dep dep/ui dep/test

test: init $(TESTBIN)
	$(TESTBIN)

$(BIN): init $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(BIN)
	$(BIN) 

$(TESTBIN): $(TEST_OBJ)
	$(CC) $(FLAGS) $(TEST_OBJ) -o $(TESTBIN)

-include $(DEP)

obj/%.o:
	mkdir -p $(@D)
	$(CC) $(FLAGS) -c $< -o $@

dep/%.d: src/%.cpp
	mkdir -p $(@D)
	$(CC) $(INCLUDE) -MM $< -MT obj/$*.o -MF $@

dep/%.d: testing/%.cpp
	mkdir -p $(@D)
	$(CC) $(INCLUDE) -MM $< -MT obj/$*.o -MF $@
