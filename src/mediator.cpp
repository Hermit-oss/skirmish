#include <fstream>
#include <filesystem>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sstream>
#include <vector>

const std::filesystem::path parentDir = std::filesystem::current_path().parent_path();
const std::filesystem::path defFile = parentDir / "build" / "defensive.o";
const std::filesystem::path offFile = parentDir / "build" / "offensive.o";
const std::filesystem::path mapFile = parentDir / "data" / "map.txt";
const std::filesystem::path statusFile = parentDir / "data" / "status.txt";
const std::filesystem::path ordersFile = parentDir / "data" / "orders.txt";
const int numTurns = 5;
const int timeLimitInSeconds = 10;

const bool player1Id = 0;
const bool player2Id = 1;

void launchPlayer(const std::string& playerAiFile, const std::string& playerName) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process - execute player AI
        execlp(("./" + playerAiFile).c_str(), playerAiFile.c_str(), mapFile.c_str(),
               statusFile.c_str(), ordersFile.c_str(), nullptr);

        // Execution should not reach this point unless execlp fails
        std::cerr << "Failed to execute player AI: " << playerAiFile << std::endl;
        exit(1);
    } else if (pid > 0) {
        // Parent process - wait for player AI to finish with a time limit
        sleep(timeLimitInSeconds);
        int status;
        pid_t result = waitpid(pid, &status, WNOHANG);
        if (result == 0) {
            // Player AI did not finish within the time limit
            std::cout << "Game over! " << playerName << " exceeded the time limit." << std::endl;
            // Perform game over actions, declare the other player as the winner, etc.
        } else if (result == -1) {
            // Error occurred while waiting for player AI
            std::cerr << "Failed to wait for player AI." << std::endl;
            // Handle the error accordingly
        } else {
            // Player AI finished within the time limit
            std::cout << playerName << " finished its turn." << std::endl;
            // Proceed with the game logic, next player's turn, etc.
        }
    }
}

int main() {
    std::cout << "==== Game Start ====" << std::endl;

    for (int turn = 0; turn < numTurns; ++turn) {
        std::cout << "=== Turn " << (turn + 1) << " ===" << std::endl;

        // Player 1's turn
        launchPlayer(defFile, "Player 1");

        // Player 2's turn
        launchPlayer(offFile, "Player 2");
    }

    std::cout << "==== Game Finished ====" << std::endl;

    return 0;
}
