# Skirmish

Skirmish is a turn-based battlefield simulator where AI controls units on a grid-based map. The entire simulation takes place within three text files: map.txt, orders.txt, and status.txt.

## Compilation

To compile the Skirmish game, follow these steps:

1. Clone the Skirmish repository to your local machine.
2. Open a terminal and navigate to the root directory of the repository.
3. Run the following commands to compile the simulator:
```
make defensive
make offensive
make
```
This commands will compile the source code and generate the `Skirmish` executable file in the root directory.

## Usage

To launch the Skirmish simulator, navigate to the directory where the `Skirmish` executable is located. Run the following command:
```
./Skirmish
```
This will start the simulation.

## Instructions (TODO)

### Functioning

### Map.txt file

### Orders.txt file

### Status.txt file

## Dependencies

The Skirmish simulator has the following dependencies:

- C++17 or later
- GNU Make

Make sure you have a compatible C++ compiler and GNU Make installed on your system before compiling the simulator.

The simulator has been written and tested on Linux Mint 21.1 Cinnamon.

## TODO

- Fix the AI to recpect the rules
- Fix the unit position assignment
- Create two separate AIs (as of right now, both defensive and offensive work the same)