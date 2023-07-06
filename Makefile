# Compiler
CXX := g++
# Compiler flags
CXXFLAGS := -std=c++17 -lpthread -Wall -Wextra -pedantic -O2
# Directory structure
SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build
DATA_DIR := data

# Source files
MAP_SRC := $(SRC_DIR)/map.cpp
UNIT_SRC := $(SRC_DIR)/unit.cpp
PLAYER_SRC := $(SRC_DIR)/player.cpp

# Object files
MAP_OBJ := $(BUILD_DIR)/map.o
UNIT_OBJ := $(BUILD_DIR)/unit.o
PLAYER_OBJ := $(BUILD_DIR)/player.o

# Executable
EXECUTABLE := Skirmish
DEFENSIVE_EXECUTABLE := $(BUILD_DIR)/defensive
OFFENSIVE_EXECUTABLE := $(BUILD_DIR)/offensive

all: $(EXECUTABLE)

$(EXECUTABLE): $(BUILD_DIR)/mediator.o $(MAP_OBJ) $(UNIT_OBJ) $(PLAYER_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(MAP_OBJ): $(MAP_SRC)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(UNIT_OBJ): $(UNIT_SRC)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(PLAYER_OBJ): $(PLAYER_SRC)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BUILD_DIR)/mediator.o: $(SRC_DIR)/mediator.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

defensive: $(DEFENSIVE_EXECUTABLE)

$(DEFENSIVE_EXECUTABLE): $(BUILD_DIR)/defensive.o $(MAP_OBJ) $(UNIT_OBJ) $(PLAYER_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/defensive.o: $(SRC_DIR)/defensive.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

offensive: $(OFFENSIVE_EXECUTABLE)

$(OFFENSIVE_EXECUTABLE): $(BUILD_DIR)/offensive.o $(MAP_OBJ) $(UNIT_OBJ) $(PLAYER_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/offensive.o: $(SRC_DIR)/offensive.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*
	rm -f $(EXECUTABLE) $(DEFENSIVE_EXECUTABLE) $(OFFENSIVE_EXECUTABLE)

.PHONY: all defensive offensive clean
