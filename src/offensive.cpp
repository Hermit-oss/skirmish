#include <iostream>
#include <random>
#include <sstream>
#include <chrono>
#include <thread>
#include <queue>
#include <atomic>
#include "player.hpp"

// Function to perform pathfinding using Breadth-First Search (BFS)
std::vector<std::vector<int>> performBFS(const Map& map, unsigned short startX, unsigned short startY) {
    unsigned int width = map.getWidth();
    unsigned int height = map.getHeight();
    std::vector<std::vector<int>> distance(height, std::vector<int>(width, -1));  // Initialize distances to -1 (unreachable)
    std::queue<std::pair<unsigned short, unsigned short>> q;

    // Starting position has distance 0
    distance[startY][startX] = 0;
    q.push({startX, startY});

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        // Check neighboring cells (up, down, left, right)
        std::vector<std::pair<int, int>> neighbors = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (const auto& [dx, dy] : neighbors) {
            unsigned short nx = x + dx;
            unsigned short ny = y + dy;

            // Check if the neighboring cell is within the map boundaries and reachable
            if (nx >= 0 && nx < width && ny >= 0 && ny < height && map.getCell(nx, ny) != '9') {
                if (distance[ny][nx] == -1) {
                    // Update the distance and enqueue the neighboring cell
                    distance[ny][nx] = distance[y][x] + 1;
                    q.push({nx, ny});
                }
            }
        }
    }

    return distance;
}

// Function to find the nearest object using pathfinding
std::pair<unsigned short, unsigned short> findSpecifiedObject(const Map& map, unsigned short startX, unsigned short startY, char object) {
    unsigned int width = map.getWidth();
    unsigned int height = map.getHeight();

    // Perform BFS to calculate distances from the starting position
    std::vector<std::vector<int>> distance = performBFS(map, startX, startY);

    // Find the object based on the calculated distances
    unsigned short nearestX = 0;
    unsigned short nearestY = 0;
    int nearestDistance = -1;

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            if (map.getCell(x, y) == object && (nearestDistance == -1 || distance[y][x] < nearestDistance)) {
                nearestX = x;
                nearestY = y;
                nearestDistance = distance[y][x];
            }
        }
    }

    return {nearestX, nearestY};
}

// Mapping of abbreviated unit types to full names
const std::unordered_map<char, std::string> unitTypeMap = {
    {'B', "Base"},
    {'W', "Worker"},
    {'S', "Swordsman"},
    {'K', "Knight"},
    {'R', "Ram"},
    {'C', "Catapult"},
    {'P', "Pikeman"},
    {'A', "Archer"}
};

// Function to read the status file
Player readStatusFile(std::ifstream& statusFileStream) {
    Player player(0, "Player 1", 0); // Create a player object with default values

    if (statusFileStream.is_open()) {
        // Read the first line containing the amount of gold
        std::string line;
        if (std::getline(statusFileStream, line)) {
            std::istringstream iss(line);
            unsigned int gold;
            iss >> gold;
            player.setGold(gold);
        }

        // Read the player and enemy details
        std::string prefix = "P";
        std::string unitType, dummy;
        int id, x, y, hp;

        while (std::getline(statusFileStream, line)) {
            if (line.substr(0, prefix.size()) == prefix) {
                std::istringstream iss(line);
                iss >> dummy >> unitType >> id >> x >> y >> hp;

                // Map the abbreviated unit type to the full name
                if (unitTypeMap.find(unitType[0]) == unitTypeMap.end()) {
                    throw std::runtime_error("Invalid unit type: " + unitType);
                }
                std::string fullName = unitTypeMap.at(unitType[0]);

                Unit unit(0, id, fullName);
                unit.setPosition(x, y);
                unit.takeDamage(unit.getHealth() - hp);
                player.addUnitToPlayerUnits(unit);
            }
        }
    }

    return player;
}

Player getEnemyUnits(std::ifstream& statusFileStream) {
    Player player(1, "Player 2", 0); // Create a player object with default values

    if (statusFileStream.is_open()) {
        // Read the first line containing the amount of gold
        std::string line;
        if (std::getline(statusFileStream, line)) {
            std::istringstream iss(line);
        }
        // Read the player and enemy details
        std::string prefix = "E";
        std::string unitType, dummy;
        int id, x, y, hp;

        while (std::getline(statusFileStream, line)) {
            if (line.substr(0, prefix.size()) == prefix) {
                std::istringstream iss(line);
                iss >> dummy >> unitType >> id >> x >> y >> hp;

                // Map the abbreviated unit type to the full name
                if (unitTypeMap.find(unitType[0]) == unitTypeMap.end()) {
                    throw std::runtime_error("Invalid unit type: " + unitType);
                }
                std::string fullName = unitTypeMap.at(unitType[0]);

                Unit unit(1, id, fullName);
                unit.setPosition(x, y);
                unit.takeDamage(unit.getHealth() - hp);
                player.addUnitToPlayerUnits(unit);
            }
        }
    }
    return player;
}

void performTurn(std::ifstream& mapFile, std::ifstream& statusFile, std::ofstream& ordersFile) {
    // Read the map file and create a Map object
    Map map(mapFile);

    // Read the status file and create a Player object
    Player player = readStatusFile(statusFile);
    // Seek the status file back to the beginning
    statusFile.clear();
    statusFile.seekg(0, std::ios::beg);
    // Read the status file and create an Enemy object
    Player enemy = getEnemyUnits(statusFile);


    unsigned short highestId = 0;
    for (auto& playerUnit : player.getPlayerUnits()) {
        for (auto& enemyUnit : enemy.getPlayerUnits()) {
            if (highestId < playerUnit.getId()) {
                highestId = playerUnit.getId();
            }
            if (highestId < enemyUnit.getId()) {
                highestId = enemyUnit.getId();
            }
        }

    }

    // Generate random numbers for making decisions
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    // Process each unit of the player
    for (auto& unit : player.getPlayerUnits()) {
        // Check if the unit is a base
        if (unit.getName() == "Base") {
            // Check if the base is not making a unit
            if (!unit.getCurrentCreation()) {
                // Generate a random number to decide whether to make a unit
                int makeUnit = dis(gen);
                if (makeUnit == 1) {
                    std::vector<std::string> unitTypes = {"Worker", "Swordsman", "Knight", "Ram", "Catapult", "Pikeman", "Archer"};
                    std::uniform_int_distribution<> disUnit(0, unitTypes.size() - 1);
                    std::string unitType = unitTypes[disUnit(gen)];
                    // Write the order to make a unit
                    ordersFile << unit.getId() << " B " << unitType[0] << std::endl;

                    // Make a unit
                    unit.createUnit(Unit(false, highestId+1, unitType));
                    highestId++;
                }
            }
        } else if (unit.getName() == "Worker") {
            // Find the nearest mine using pathfinding
            auto [mineX, mineY] = findSpecifiedObject(map, unit.getPositionX(), unit.getPositionY(), '6');

            // Write the order to move the worker towards the mine
            ordersFile << unit.getId() << " M " << mineX << " " << mineY << std::endl;

            // Move the worker towards the mine
            unit.moveAction(mineX, mineY, enemy.getPlayerUnits(), map);
        } else {
            // Find the nearest enemy base using pathfinding
            auto [baseX, baseY] = findSpecifiedObject(map, unit.getPositionX(), unit.getPositionY(), '2');

            // Use the pathfinding algorithm to find the shortest path to the enemy base
            std::vector<std::vector<int>> path = performBFS(map, unit.getPositionX(), unit.getPositionY());

            // Move the unit along the path and attack any enemy units encountered
            for (const auto& cell : path) {
                // Check if there are enemy units at the current position
                for (const Unit& enemyUnit : player.getPlayerUnits()) {
                    if (!enemyUnit.getOwner() && enemyUnit.getPositionX() == cell[0] && enemyUnit.getPositionY() == cell[1]) {
                        // Write the order to attack the enemy unit
                        ordersFile << unit.getId() << " A " << enemyUnit.getId() << std::endl;

                        // Attack the enemy unit
                        unit.attackAction(enemyUnit.getId(), enemy.getPlayerUnits());
                    }
                }

                // Write the order to move to the next position
                ordersFile << unit.getId() << " M " << cell[0] << " " << cell[1] << std::endl;

                // Move the unit to the next position
                unit.moveAction(cell[0], cell[1], player.getPlayerUnits(), map);
            }
        }
    }
}

void performTurnWithTimeout(std::ifstream& mapFile, std::ifstream& statusFile, std::ofstream& ordersFile, int timeoutInSeconds) {
    bool isTimeout = false;
    bool isTurnCompleted = false;

    // Create a flag to track if the turn thread has finished
    std::atomic<bool> turnFinished(false);


    // Create a thread to perform the turn
    std::thread turnThread([&]() {
        // Call the performTurn function
        performTurn(mapFile, statusFile, ordersFile);
        isTurnCompleted = true;
        turnFinished.store(true);
    });

    // Create a thread to monitor the timeout
    std::thread timeoutThread([&]() {
        // Sleep for the specified timeout duration
        std::this_thread::sleep_for(std::chrono::seconds(timeoutInSeconds));

        // If the timeout expires and the turn is not completed, set the flag
        if (!isTurnCompleted) {
            isTimeout = true;
        }

        // Notify the turn thread to finish early
        turnFinished.store(true);
    });

    // Wait for the turn thread to complete or the timeout to expire
    turnThread.join();

    // Check if the turn thread finished before the timeout
    if (!isTimeout && !turnFinished.load()) {
        std::cout << "Turn completed early!" << std::endl;
        return;  // Finish the function early
    }

    // Join the timeout thread
    timeoutThread.join();

    // Check if the timeout occurred
    if (isTimeout) {
        throw std::runtime_error("Timeout occurred!");
    }
}


int main(int argc, char* argv[]) {
    if (argc < 4 || argc > 5) {
        std::cerr << "Invalid amount of arguments. Usage: ./defensive.o <map file> <status file> <orders file> [time limit]" << std::endl;
        return 1;
    }

    std::string mapFile = argv[1];
    std::string statusFile = argv[2];
    std::string ordersFile = argv[3];
    unsigned int timeLimit = 5;

    if (argc == 5) {
        timeLimit = std::stoi(argv[4]);
    }

    // Open files
    std::ifstream mapFileStream(mapFile);
    std::ifstream statusFileStream(statusFile);
    std::ofstream ordersFileStream(ordersFile);

    if (!mapFileStream || !statusFileStream || !ordersFileStream) {
        std::cerr << "Failed to open one or more files." << std::endl;
        return 1;
    }

    try {
        // Call the performTurnWithTimeout function with the specified time limit
        performTurnWithTimeout(mapFileStream, statusFileStream, ordersFileStream, timeLimit);

        // The performTurn function completed within the specified time limit
        std::cout << "Turn completed successfully!" << std::endl;
    } catch (const std::runtime_error& e) {
        // Handle the timeout error
        std::cout << "Error: " << e.what() << std::endl;
    }

    // Close your input and output files as needed
    mapFileStream.close();
    statusFileStream.close();
    ordersFileStream.close();

    return 0;
}