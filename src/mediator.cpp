#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <filesystem>
#include "player.hpp"

namespace fs = std::filesystem;

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
    // First, replace the first line with the playerGold
    statusFile.seekp(0); // Move the write position to the beginning of the file
    statusFile << player.getGold() << std::endl;
    
    // Then, switch P's to E's and E's to P's in each line
    std::string line;
    while (std::getline(statusFile, line)) {
        if (!line.empty()) {
            if (line[0] == 'P')
                line[0] = 'E';
            else if (line[0] == 'E')
                line[0] = 'P';
        }
        // Move the read position back to the beginning of the line
        statusFile.seekg(-(static_cast<int>(line.length()) + 1), std::ios_base::cur);
        // Write the modified line back to the file
        statusFile << line << std::endl;
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
    const unsigned short timeLimit = 10;
    // Other
    const unsigned short numberOfTurnsPerPlayer = 1000;

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

    // Status file stream
    std::fstream statusFileStream(statusFile, std::ios::in | std::ios::out);
    if (!statusFileStream) {
        std::cerr << "Failed to open the status file." << std::endl;
        return 1;
    }

    std::cout << "==== Game Start ====" << std::endl;
    for (int turn = 0; turn < numberOfTurnsPerPlayer; turn++) {
        std::cout << "=== Turn " << (turn + 1) << " ===" << std::endl;


        // Player 1's turn
        int player1Result = system(player1Command.c_str());
        if (player1Result != 0) {
            std::cerr << "Player 1's turn failed with exit code: " << player1Result << std::endl;
            return 1;
        }

        switchStatus(statusFileStream, player2);

        // Player 2's turn
        int player2Result = system(player2Command.c_str());
        if (player2Result != 0) {
            std::cerr << "Player 2's turn failed with exit code: " << player2Result << std::endl;
            return 1;
        }

        switchStatus(statusFileStream, player1);
    }
    std::cout << "==== Game Finished ====" << std::endl;

    return 0;
}
