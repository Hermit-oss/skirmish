#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

/**
 * @class Map
 * @brief Represents a game map with grid-based cells.
 *
 * The Map class provides functionality to load and access a game map composed of
 * grid-based cells. It allows retrieving the dimensions of the map, as well as
 * accessing individual cells within the map grid.
 */
class Map {
private:
    std::vector<std::vector<char>> grid; /**< The grid of cells representing the map. */

public:
    /**
     * @brief Constructs a Map object by loading map data from a file.
     * @param filename The name of the file containing the map data.
     * @throw std::runtime_error If the map data fails to load from the file.
     */
    Map(const std::string& filename);

    /**
     * @brief Retrieves the width of the map.
     * @return The width of the map in number of cells.
     */
    unsigned int getWidth() const;

    /**
     * @brief Retrieves the height of the map.
     * @return The height of the map in number of cells.
     */
    unsigned int getHeight() const;

    /**
     * @brief Retrieves the character representing the cell at the given coordinates.
     * @param x The x-coordinate of the cell.
     * @param y The y-coordinate of the cell.
     * @return The character representing the cell at the given coordinates.
     * @throw std::out_of_range If the provided coordinates are out of bounds.
     */
    char getCell(unsigned int x, unsigned int y) const;

private:
    /**
     * @brief Loads the map data from a file.
     * @param filename The name of the file containing the map data.
     * @throw std::runtime_error If the map data fails to load from the file.
     */
    void loadMapFromFile(const std::string& filename);

    /**
     * @brief Validates the loaded map data.
     * @throw std::runtime_error If the loaded map data is invalid or inconsistent.
     */
    void validateMapData() const;

    /**
     * @brief Checks if the provided character is a valid cell character.
     * @param c The character to check.
     * @return True if the character is valid, false otherwise.
     */
    bool isValidCellCharacter(char c) const;

    /**
     * @brief Checks if the provided dimensions are valid.
     * @param width The width of the map.
     * @param height The height of the map.
     * @return True if the dimensions are valid, false otherwise.
     */
    bool isValidDimensions(unsigned int width, unsigned int height) const;
};

#endif  // MAP_HPP
