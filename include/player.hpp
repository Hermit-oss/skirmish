// player.hpp

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include <string>
#include "unit.hpp"

/**
 * @class Player
 * @brief Represents a player in the game.
 *
 * The Player class represents a player in the game. It holds information about
 * the player's ID, name, and units.
 */
class Player {
private:
    bool playerId; /**< The ID of the player. */
    std::string playerName; /**< The name of the player. */
    std::vector<Unit> playerUnits; /**< The units owned by the player. */

public:
    /**
     * @brief Constructs a Player object with the specified ID and name.
     * @param id The ID of the player.
     * @param name The name of the player.
     */
    Player(bool id, const std::string& name);

    /**
     * @brief Retrieves the ID of the player.
     * @return The ID of the player.
     */
    bool getID() const;

    /**
     * @brief Retrieves the name of the player.
     * @return The name of the player.
     */
    std::string getName() const;

    /**
     * @brief Retrieves the units owned by the player.
     * @return The units owned by the player.
     */
    std::vector<Unit> getPlayerUnits() const;

    /**
     * @brief Adds a unit to the units owned by the player.
     * @param unit The unit to add.
     */
    void addUnitToPlayerUnits(const Unit& unit);
};

#endif // PLAYER_HPP
