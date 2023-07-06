#include "map.hpp"

Map::Map(const std::string& filename) {
    loadMapFromFile(filename);
}

Map::Map(std::ifstream& file) {
    if (!file) {
        throw std::runtime_error("Failed to open map file");
    }

    // Read each line from the file and store it in the grid
    std::string line;
    while (std::getline(file, line)) {
        std::vector<char> row;
        for (char c : line) {
            if (!isValidCellCharacter(c)) {
                throw std::runtime_error("Invalid character: " + c);
            }
            row.push_back(c);
        }
        grid.push_back(row);
    }

    validateMapData();
}

unsigned int Map::getWidth() const {
    return grid.empty() ? 0 : grid[0].size();
}

unsigned int Map::getHeight() const {
    return grid.size();
}

char Map::getCell(unsigned int x, unsigned int y) const {
    if (x >= getWidth() || y >= getHeight()) {
        throw std::out_of_range("Invalid cell coordinates.");
    }
    return grid[y][x];
}

std::pair<unsigned int, unsigned int> Map::getBasePosition(char baseCell) const {
    for (unsigned int y = 0; y < getHeight(); ++y) {
        for (unsigned int x = 0; x < getWidth(); ++x) {
            char cell = getCell(x, y);
            if (cell == baseCell) {
                return std::make_pair(x, y);
            }
        }
    }

    throw std::runtime_error("No base position found in the map.");
}

void Map::loadMapFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open map file: " + filename);
    }

    // Read each line from the file and store it in the grid
    std::string line;
    while (std::getline(file, line)) {
        std::vector<char> row;
        for (char c : line) {
            if (!isValidCellCharacter(c)) {
                throw std::runtime_error("Invalid character: " + c);
            }
            row.push_back(c);
        }
        grid.push_back(row);
    }

    validateMapData();
}

void Map::validateMapData() const {
    unsigned int width = getWidth();
    unsigned int height = getHeight();

    // Ensure all rows have the same width
    for (const auto& row : grid) {
        if (row.size() != width) {
            throw std::runtime_error("Inconsistent row width in the map data.");
        }
    }

    // Check if map dimensions are valid
    if (!isValidDimensions(width, height)) {
        throw std::runtime_error("Invalid map dimensions.");
    }
}

bool Map::isValidCellCharacter(char c) const {
    // Define the valid cell characters
    std::string validCharacters = "01269";

    // Check if the character is in the valid characters set
    return validCharacters.find(c) != std::string::npos;
}

bool Map::isValidDimensions(unsigned int width, unsigned int height) const {
    // Check if the dimensions are positive and non-zero
    return width > 0 && height > 0;
}
