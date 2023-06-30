#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    // Data files
    const fs::path mapFile = "../data/map.txt";
    const fs::path statusFile = "../data/status.txt";
    const fs::path ordersFile = "../data/orders.txt";
    // Player AI files
    const fs::path player1File = "../build/defensive.o";
    const fs::path player2File = "../build/offensive.o";
    // Time limit in seconds
    const unsigned short timeLimit = 10;
    // Other
    const unsigned short numberOfTurnsPerPlayer = 1000;

    // Check file existence
    if (!fs::exists(mapFile) || !fs::exists(statusFile) || !fs::exists(ordersFile) ||
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

    std::cout << "==== Game Start ====" << std::endl;
    for (int turn = 0; turn < numberOfTurnsPerPlayer; ++turn) {
        std::cout << "=== Turn " << (turn + 1) << " ===" << std::endl;

        // Player 1's turn
        int player1Result = system(player1Command.c_str());
        if (player1Result != 0) {
            std::cerr << "Player 1's turn failed with exit code: " << player1Result << std::endl;
            return 1;
        }

        // Player 2's turn
        int player2Result = system(player2Command.c_str());
        if (player2Result != 0) {
            std::cerr << "Player 2's turn failed with exit code: " << player2Result << std::endl;
            return 1;
        }
    }
    std::cout << "==== Game Finished ====" << std::endl;

    return 0;
}
