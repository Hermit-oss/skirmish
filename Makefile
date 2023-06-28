CC := g++
CFLAGS := -std=c++17 -Wall -Wextra
BUILD_DIR := build
SRC_DIR := src
INCLUDE_DIR := include
DATA_DIR := data
EXECUTABLE := skirmish

# List of source files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
# List of object files
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)
