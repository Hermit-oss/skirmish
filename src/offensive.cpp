#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <filesystem>

void performTurn(std::ifstream& mapFile, std::ifstream& statusFile, std::ofstream& ordersFile) {
    
    /* AI not done*/
    
    ordersFile << "1 M 10 10" << std::endl;

}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Insufficient number of arguments. Expected 5, got " << argc << std::endl;
        return 1;
    }

    // Open map file
    std::filesystem::path mapFilePath = argv[1];
    std::ifstream mapFile;
    mapFile.open(mapFilePath);
    if (!mapFile) {
        std::cerr << "Failed to open map file at: " << argv[1] << std::endl;
        return 1;
    }

    // Open status file
    std::filesystem::path statusFilePath = argv[2];
    std::ifstream statusFile;
    statusFile.open(statusFilePath);
    if (!statusFile) {
        std::cerr << "Failed to open status file at: " << argv[2] << std::endl;
        return 1;
    }

    // Open orders file
    std::filesystem::path ordersFilePath = argv[3];
    std::ofstream ordersFile;
    ordersFile.open(ordersFilePath);
    if (!ordersFile) {
        std::cerr << "Failed to open orders file at: " << argv[3] << std::endl;
        return 1;
    }

    // Perform the turn
    performTurn(mapFile, statusFile, ordersFile);

    return 0;
}
