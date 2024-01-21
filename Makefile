CXX       := g++
CXX_FLAGS := -Wall -Wextra -pedantic -std=c++20 -ggdb

BIN     := bin
SRC     := src
INCLUDE := include

LIBRARIES   := -lfmt
EXECUTABLE  := main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*
