#include <iostream>
#include <cstdlib>
#include <filesystem>
#include "player.hpp"

namespace fs = std::filesystem;

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

void initializeStatus(const fs::path& statusFilePath, Player& player1, Player& player2) {
    // Delete the file if it exists
    if (fs::exists(statusFilePath)) {
        fs::remove(statusFilePath);
    }

    // Create a new status file
    std::ofstream statusFile(statusFilePath);

    // Set the first line to player1's gold
    statusFile << player1.getGold() << std::endl;

    // Write player1's unit information to the second line
    const std::vector<Unit>& player1Units = player1.getPlayerUnits();
    if (!player1Units.empty()) {
        const Unit& unit = player1Units[0];
        statusFile << "P B " << unit.getId() << " "
                   << unit.getPositionX() << " " << unit.getPositionY() << " "
                   << unit.getHealth() << " 0" << std::endl;
    }

    // Write player2's unit information to the third line
    const std::vector<Unit>& player2Units = player2.getPlayerUnits();
    if (!player2Units.empty()) {
        const Unit& unit = player2Units[0];
        statusFile << "E B " << unit.getId() << " "
                   << unit.getPositionX() << " " << unit.getPositionY() << " "
                   << unit.getHealth() << " 0" << std::endl;
    }

    // Close the status file
    statusFile.close();
}

void switchStatus(std::fstream& statusFile, Player& player) {
    // Read the file line by line and update the contents
    std::ostringstream updatedContents;
    std::string line;
    bool firstLine = true;
    while (std::getline(statusFile, line)) {
        if (!line.empty()) {
            if (firstLine) {
                updatedContents << player.getGold() << '\n';
                firstLine = false;
            } else {
                char firstChar = line[0];
                if (firstChar == 'P')
                    firstChar = 'E';
                else if (firstChar == 'E')
                    firstChar = 'P';
                line[0] = firstChar;
                updatedContents << line << '\n';
            }
        }
    }
    
    // Move the write position to the beginning of the file and write the updated contents
    statusFile.clear(); // Clear the end-of-file flag
    statusFile.seekp(0);
    statusFile << updatedContents.str();
    statusFile.flush(); // Flush the changes to the file
    statusFile.seekg(0); // Move the read position back to the beginning of the file
}

unsigned short getHighestID(Player& player, Player& enemy) {
    unsigned short highestID = 0;
    for (auto& unit : player.getPlayerUnits()) {
        unsigned short newID = unit.getId();
        if (newID > highestID) {
            highestID = newID;
        }
    }
    for (auto& unit : enemy.getPlayerUnits()) {
        unsigned short newID = unit.getId();
        if (newID > highestID) {
            highestID = newID;
        }
    }
    return highestID;
}

void analyzeTurn(std::ifstream& ordersFile, std::fstream& statusFile, Player& player, Player& enemy, Map& map) {
    std::string line;
    while (std::getline(ordersFile, line)) {
        std::istringstream iss(line);
        int unitId;
        std::string action;

        if (!(iss >> unitId >> action)) {
            // Invalid order format, skip the line
            continue;
        }

        // Handle different actions
        if (action == "B") {
            char unitTypeAbbreviation;
            if (iss >> unitTypeAbbreviation) {
                // Build unit action
                std::string unitType;
                if (unitTypeMap.find(unitTypeAbbreviation) != unitTypeMap.end()) {
                    unitType = unitTypeMap.at(unitTypeAbbreviation);
                    Unit newUnit(false, getHighestID(player, enemy) + 1, unitType);
                    player.getPlayerUnits()[0].createUnit(newUnit);
                    // Update status file
                    statusFile << "P " << unitTypeAbbreviation << " " << newUnit.getId() << " 0 0 " << newUnit.getHealth() << std::endl;
                }
            }
        } else if (action == "M") {
            unsigned short x, y;
            if (iss >> x >> y) {
                // Move unit action
                player.getUnitByID(unitId).moveAction(x, y, enemy.getPlayerUnits(), map);
                // Update status file
                statusFile  << "P " << player.getUnitByID(unitId).getInitial() << " " << unitId << " " 
                            << player.getUnitByID(unitId).getPositionX() << " " << player.getUnitByID(unitId).getPositionY() 
                            << " " << player.getUnitByID(unitId).getHealth() << std::endl;
            }
        } else if (action == "A") {
            int targetId;
            if (iss >> targetId) {
                // Attack unit action
                player.getUnitByID(unitId).attackAction(targetId, enemy.getPlayerUnits());
                // Update status file
                statusFile  << "P " << player.getUnitByID(unitId).getInitial() << " " << unitId << " " 
                            << player.getUnitByID(unitId).getPositionX() << " " << player.getUnitByID(unitId).getPositionY() 
                            << " " << player.getUnitByID(unitId).getHealth() << std::endl;
                statusFile  << "E " << enemy.getUnitByID(targetId).getInitial() << " " << targetId << " " 
                            << enemy.getUnitByID(targetId).getPositionX() << " " << enemy.getUnitByID(targetId).getPositionY() 
                            << " " << enemy.getUnitByID(targetId).getHealth() << std::endl;
            }
        }
    }
}

int main() {
    // Data files paths
    const fs::path mapFile = "data/map.txt";
    const fs::path statusFile = "data/status.txt";
    const fs::path ordersFile = "data/orders.txt";
    // Player AI files
    const fs::path player1File = "build/defensive";
    const fs::path player2File = "build/offensive";
    // Time limit in seconds
    const unsigned short timeLimit = 1;
    // Other
    const unsigned short numberOfTurnsPerPlayer = 10;

    // Check file existence
    if (!fs::exists(mapFile) || !fs::exists(ordersFile) ||
        !fs::exists(player1File) || !fs::exists(player2File)) {
        std::cerr << "One or more required files do not exist." << std::endl;
        return 1;
    }

    std::string player1Command = player1File.string() + " " + mapFile.string() + " " + statusFile.string() + " " + ordersFile.string();
    std::string player2Command = player2File.string() + " " + mapFile.string() + " " + statusFile.string() + " " + ordersFile.string();

    // Append the optional time limit argument if provided
    if (timeLimit > 0) {
        player1Command += " " + std::to_string(timeLimit);
        player2Command += " " + std::to_string(timeLimit);
    }

    // Initialize map and base positions
    Map map(mapFile.string());
    std::pair<unsigned int, unsigned int>player1Base = map.getBasePosition('1');
    std::pair<unsigned int, unsigned int>player2Base = map.getBasePosition('2');

    // Initialize players
    Player player1(0, "Player 1", 2500);
    player1.addUnitToPlayerUnits(Unit(player1.getID(), player1.getID(), "Base"));
    player1.getPlayerUnits()[0].setPosition(player1Base.first, player1Base.second);
    Player player2(1, "Player 2", 2500);
    player2.addUnitToPlayerUnits(Unit(player2.getID(), player2.getID(), "Base"));
    player2.getPlayerUnits()[0].setPosition(player2Base.first, player2Base.second);

    // Initialize status file
    initializeStatus(statusFile, player1, player2);

    // Orders file stream
    std::ifstream ordersFileStream(ordersFile);
    if (!ordersFileStream) {
        std::cerr << "Failed to open the orders file." << std::endl;
        return 1;    
    }

    // Status file stream
    std::fstream statusFileStream(statusFile, std::ios::in | std::ios::out);
    if (!statusFileStream) {
        std::cerr << "Failed to open the status file." << std::endl;
        return 1;
    }

    std::cout << "==== SIMULATION START ====" << std::endl;
    for (int turn = 0; turn < numberOfTurnsPerPlayer; turn++) {
        std::cout << "=== Turn " << (turn + 1) << " ===" << std::endl;

        // Player 1's turn
        int player1Result = system(player1Command.c_str());
        if (player1Result != 0) {
            std::cerr << "Player 1's turn failed with exit code: " << player1Result << std::endl;
            return 1;
        }
        analyzeTurn(ordersFileStream, statusFileStream, player1, player2, map);
        switchStatus(statusFileStream, player2);

        // Player 2's turn
        int player2Result = system(player2Command.c_str());
        if (player2Result != 0) {
            std::cerr << "Player 2's turn failed with exit code: " << player2Result << std::endl;
            return 1;
        }
        analyzeTurn(ordersFileStream, statusFileStream, player2, player2, map);
        switchStatus(statusFileStream, player1);
    }
    std::cout << "==== SIMULATION FINISHED ====" << std::endl;

    return 0;
}
