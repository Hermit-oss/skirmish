#ifndef UNIT_H
#define UNIT_H

#include "map.hpp"
#include <unordered_map>

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

/**
 * @struct UnitAttributes
 * @brief Represents the attributes of a unit type.
 */
struct UnitAttributes {
    unsigned short health;        /**< The health of the unit. */
    unsigned short speed;         /**< The speed of the unit. */
    unsigned short cost;          /**< The cost of the unit. */
    unsigned short attackRange;   /**< The attack range of the unit. */
    unsigned short buildingTime;  /**< The building time of the unit. */
};

/**
 * @class Unit
 * @brief Represents a game unit with various attributes and behavior.
 */
class Unit {
private:
    unsigned short id;              /**< The ID of the unit. */
    unsigned short health;          /**< The health of the unit. */
    unsigned short speed;           /**< The speed of the unit. */
    unsigned short cost;            /**< The cost of the unit. */
    unsigned short attackRange;     /**< The attack range of the unit. */
    unsigned short buildingTime;    /**< The building time of the unit. */
    unsigned short position[2];     /**< The position of the unit. */
    std::string name;               /**< The name of the unit. */

    bool owner;                     /**< The owner's ID. */

    unsigned short baseSpeed;       /**< The base speed of the unit. */
    bool hasAttacked;               /**< Flag for when the unit has taken an attack action. */

    Unit* currentCreation = nullptr;/**< Pointer to the unit currently being created by the base. */

public:
    /**
     * @brief Constructor for the Unit class.
     * @param owner The owner of the unit.
     * @param id The ID of the unit.
     * @param name The name of the unit.
     * @throws std::runtime_error if the unit name is invalid and not found in the unitAttributesMap.
     */
    Unit(bool owner, unsigned short id, const std::string& name);

    // Getters

    /**
     * @brief Get the ID of the unit.
     * @return The ID of the unit.
     */
    unsigned short getId() const;

    /**
     * @brief Get the health of the unit.
     * @return The health of the unit.
     */
    unsigned short getHealth() const;

    /**
     * @brief Get the speed of the unit.
     * @return The speed of the unit.
     */
    unsigned short getSpeed() const;

    /**
     * @brief Get the cost of the unit.
     * @return The cost of the unit.
     */
    unsigned short getCost() const;

    /**
     * @brief Get the attack range of the unit.
     * @return The attack range of the unit.
     */
    unsigned short getAttackRange() const;

    /**
     * @brief Get the building time of the unit.
     * @return The building time of the unit.
     */
    unsigned short getBuildingTime() const;

    /**
     * @brief Get the X position of the unit.
     * @return The X position of the unit.
     */
    unsigned short getPositionX() const;

    /**
     * @brief Get the Y position of the unit.
     * @return The Y position of the unit.
     */
    unsigned short getPositionY() const;

    /**
     * @brief Get the name of the unit.
     * @return The name of the unit.
     */
    std::string getName() const;

    /**
     * @brief Get the owner of the unit.
     * @return The owner of the unit.
     */
    bool getOwner() const;

    /**
     * @brief Get the initial letter of the unit's name.
     * @return The initial letter of the unit's name.
     */
    char getInitial() const;

    Unit* getCurrentCreation() {
        return currentCreation;
    }

    // Setters

    void setPosition(unsigned int x, unsigned int y);

    // Other member functions

    /**
     * @brief Calculate the damage inflicted by the unit on a target unit.
     * @param target The target unit.
     * @return The damage inflicted by the unit on the target unit.
     */
    unsigned short calculateDamage(const Unit& target) const;

    /**
     * @brief Calculates the distance between the unit's current position and the specified coordinates.
     * @param x The X coordinate of the new position.
     * @param y The Y coordinate of the new position.
     * @return The distance between the current position and the specified coordinates.
     */
    unsigned short calculateDistance(unsigned short x, unsigned short y);

    /**
     * @brief Performs an attack action on a target unit with the specified ID.
     * @param targetId The ID of the target unit to attack.
     * @param units The list of units to search for the target unit.
     * @throws std::runtime_error if the target unit with the specified ID is not found.
     */
    void attackAction(unsigned short targetId, const std::vector<Unit>& units);

    /**
     * @brief Performs a move action by changing the position of the unit to the specified coordinates.
     * @param x The X coordinate of the new position.
     * @param y The Y coordinate of the new position.
     * @param units The list of units to check for collisions.
     * @param map The map object to check for obstacles.
     * @throws std::runtime_error if the movement distance exceeds the unit's speed or if the target position is an obstacle.
     */
    void moveAction(unsigned short x, unsigned short y, const std::vector<Unit>& units, const Map& map);

    /**
     * @brief Inflicts damage to the unit based on the specified amount.
     * @param amount The amount of damage to be inflicted.
     */
    void takeDamage(unsigned short amount);

    /**
     * @brief Decrease the building time of the unit by 1.
     * @return true if the building time is greater than 0 after the decrease, false otherwise.
     */
    bool buildingTick();

    /**
     * @brief Reset the unit's speed and hasAttacked flag.
     */
    void reset();

    /**
     * @brief Deploy the unit onto the map.
     * @param base The home base of the unit.
     */
    void deploy(const Unit& base);

    /**
     * @brief Check if the unit is a worker unit.
     * @return true if the unit is a worker unit, false otherwise.
     */
    bool isWorker() const;

    /**
     * @brief Check if the unit is currently on a mine tile on the map.
     * @param map The map object to check for the unit's position.
     * @return true if the worker unit is on a mine tile, false otherwise.
     */
    bool isWorkerOnMine(const Map& map) const;

    void createUnit(const Unit& unit);

private:
    /**
     * @brief Initialize the unit's attributes based on its name.
     */
    void initializeUnitAttributes();

    /**
     * @brief Map that stores the attributes for each unit type.
     */
    static const std::unordered_map<std::string, UnitAttributes> unitAttributesMap;

    /**
     * @brief Map that stores the damage values between different unit types.
     */
    static const std::unordered_map<std::string, std::unordered_map<std::string, unsigned short>> damageMap;
};

#endif  // UNIT_H
