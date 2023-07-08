#include "unit.hpp"

// Map of unit attributes for each unit name
const std::unordered_map<std::string, UnitAttributes> Unit::unitAttributesMap = {
    // Initialize the attributes for each unit type
    {"Knight", {90, 5, 400, 1, 5}},
    {"Swordsman", {60, 2, 250, 5, 3}},
    {"Archer", {40, 2, 250, 5, 3}},
    {"Pikeman", {50, 2, 200, 2, 3}},
    {"Ram", {90, 2, 500, 1, 4}},
    {"Catapult", {50, 2, 800, 7, 6}},
    {"Worker", {20, 2, 100, 1, 2}},
    {"Base", {200, 0, 0, 0, 0}}
};

// Map of damage values for each unit against other units
const std::unordered_map<std::string, std::unordered_map<std::string, unsigned short>> Unit::damageMap = {
    // Initialize the damage values for each unit against other units
    {"Knight", {{"Knight", 35}, {"Swordsman", 35}, {"Archer", 35}, {"Pikeman", 35}, {"Catapult", 35}, {"Ram", 50}, {"Worker", 35}, {"Base", 35}}},
    {"Swordsman", {{"Knight", 30}, {"Swordsman", 30}, {"Archer", 30}, {"Pikeman", 20}, {"Catapult", 20}, {"Ram", 30}, {"Worker", 30}, {"Base", 30}}},
    {"Archer", {{"Knight", 15}, {"Swordsman", 15}, {"Archer", 15}, {"Pikeman", 15}, {"Catapult", 10}, {"Ram", 10}, {"Worker", 15}, {"Base", 15}}},
    {"Pikeman", {{"Knight", 35}, {"Swordsman", 15}, {"Archer", 15}, {"Pikeman", 15}, {"Catapult", 15}, {"Ram", 10}, {"Worker", 15}, {"Base", 10}}},
    {"Catapult", {{"Knight", 40}, {"Swordsman", 40}, {"Archer", 40}, {"Pikeman", 40}, {"Catapult", 40}, {"Ram", 40}, {"Worker", 40}, {"Base", 50}}},
    {"Ram", {{"Knight", 10}, {"Swordsman", 10}, {"Archer", 10}, {"Pikeman", 10}, {"Catapult", 10}, {"Ram", 10}, {"Worker", 10}, {"Base", 50}}},
    {"Worker", {{"Knight", 5}, {"Swordsman", 5}, {"Archer", 5}, {"Pikeman", 5}, {"Catapult", 5}, {"Ram", 5}, {"Worker", 5}, {"Base", 1}}}
};

// Constructor for the Unit class
Unit::Unit(bool owner, unsigned short id, const std::string& name) : owner(owner), id(id), name(name) {
    // Check if the unit name exists in the unitAttributesMap
    auto it = unitAttributesMap.find(name);
    if (it == unitAttributesMap.end()) {
        throw std::runtime_error("Invalid unit name: " + name);
    }

    // Initialize the unit attributes
    initializeUnitAttributes();

    // Set the unit's attack action flag
    hasAttacked = false;

    // Remember the unit's base speed
    baseSpeed = speed;
}

// Getters for various unit attributes
unsigned short Unit::getId() const {
    return id;
}

unsigned short Unit::getHealth() const {
    return health;
}

unsigned short Unit::getSpeed() const {
    return speed;
}

unsigned short Unit::getCost() const {
    return cost;
}

unsigned short Unit::getAttackRange() const {
    return attackRange;
}

unsigned short Unit::getBuildingTime() const {
    return buildingTime;
}

unsigned short Unit::getPositionX() const {
    return position[0];
}

unsigned short Unit::getPositionY() const {
    return position[1];
}

std::string Unit::getName() const {
    return name;
}

bool Unit::getOwner() const {
    return owner;
}

char Unit::getInitial() const {
    if (name.empty()) {
        throw std::runtime_error("Unit's name is empty.");
    }
    return name[0];
}

void Unit::setPosition(unsigned int x, unsigned int y) {
    position[0] = x;
    position[1] = y;
}

// Calculate the damage inflicted by the current unit to the target unit
unsigned short Unit::calculateDamage(const Unit& target) const {
    const std::string& targetName = target.getName();

    // Check if the current unit's name exists in the damage map
    auto it = damageMap.find(name);
    if (it != damageMap.end()) {
        const auto& unitDamageMap = it->second;
        
        // Check if the target unit's name exists in the damage map for the current unit
        auto damageIt = unitDamageMap.find(targetName);
        if (damageIt != unitDamageMap.end()) {
            // Return the damage value for the target unit
            return damageIt->second;
        }
    }

    // Return 0 if no damage value is found
    return 0;
}

// Calculate the distance between the unit's current position and the target position (x, y)
unsigned short Unit::calculateDistance(unsigned short x, unsigned short y) {
    return std::abs(position[0] - x) + std::abs(position[1] - y);
}

// Initialize the unit's attributes based on the unitAttributesMap
void Unit::initializeUnitAttributes() {
    // Find the attributes for the current unit name in the unitAttributesMap
    auto it = unitAttributesMap.find(name);
    if (it != unitAttributesMap.end()) {
        // Retrieve the attributes for the current unit
        const UnitAttributes& attributes = it->second;

        // Set the unit's attributes based on the retrieved values
        health = attributes.health;
        speed = attributes.speed;
        cost = attributes.cost;
        attackRange = attributes.attackRange;
        buildingTime = attributes.buildingTime;
    }
}

// Perform an attack action on the target unit with the specified ID
void Unit::attackAction(unsigned short targetId, const std::vector<Unit>& units) {
    if (name == "Base") {
        throw std::runtime_error("Base unit cannot perform attack action. ");
    }
    if (speed == 0) {
        throw std::runtime_error("Unit cannot attack. Speed is 0.");
    }
    if (hasAttacked) {
        throw std::runtime_error("Unit can only attack once.");
    }
    
    // Find the target unit with the specified ID
    const Unit* tempTargetUnit = nullptr;
    for (const Unit& unit : units) {
        if (unit.getId() == targetId) {
            tempTargetUnit = &unit;
            break;
        }
    }

    Unit* targetUnit = const_cast<Unit*>(tempTargetUnit);

    if (targetUnit) {
        // Throw an error when trying to attack an ally
        if (owner == targetUnit->owner) {
            throw std::runtime_error("A unit cannot attack their allies.");
        }

        // Calculate the distance between the unit's current position and the target unit's position
        unsigned short distance = calculateDistance(targetUnit->getPositionX(), targetUnit->getPositionY());

        // Check if the target unit is within the attack range
        if (distance > attackRange) {
            throw std::runtime_error("Target unit is out of attack range.");
        }

        // Calculate the damage to be dealt to the target unit
        unsigned short damage = calculateDamage(*targetUnit);

        // Deal the calculated amount of damage to the target unit
        targetUnit->takeDamage(damage);

        // Decrease the unit's speed by 1 after a successful attack
        speed -= 1;

        // Set the hasAttacked flag to true
        hasAttacked = true;
    } else {
        throw std::runtime_error("Target unit with ID " + std::to_string(targetId) + " not found.");
    }
}

// Perform a move action to the specified position (x, y)
void Unit::moveAction(unsigned short x, unsigned short y, const std::vector<Unit>& units, const Map& map) {
    unsigned short distance = calculateDistance(x, y);

    if (name == "Base") {
        throw std::runtime_error("Base unit cannot perform move action. ");
    }
    if (distance > speed) {
        throw std::runtime_error("Movement distance exceeds the unit's speed.");
    }

    // Check if the target position is within the map's boundaries
    if (x >= map.getWidth() || y >= map.getHeight()) {
        throw std::runtime_error("Target position is outside the map's boundaries.");
    }

    // Check if the target position is an obstacle
    if (map.getCell(x, y) == '9') {
        throw std::runtime_error("Target position is an obstacle and cannot be moved to.");
    }

    // Check if the target position is occupied by an enemy unit
    for (const Unit& unit : units) {
        if (unit.getPositionX() == x && unit.getPositionY() == y && unit.getOwner() != owner) {
            throw std::runtime_error("Cannot enter the enemy's unit space.");
        }
    }

    position[0] = x;
    position[1] = y;

    speed -= distance;
}

// Take the specified amount of damage
void Unit::takeDamage(unsigned short amount) {
    if (amount >= health) {
        health = 0;
    } else {
        health -= amount;
    }
}

// Perform a building tick for the unit, reducing its building time by 1
bool Unit::buildingTick() {
    if (name == "Base") {
        throw std::runtime_error("Base unit cannot be built. ");
    }
    if (buildingTime > 0) {
        --buildingTime;
    }
    
    return buildingTime == 0;
}

// Reset the unit's attributes to their initial values
void Unit::reset() {
    speed = baseSpeed;
    hasAttacked = false;
}

// Deploy the unit on the home base's space
void Unit::deploy(const Unit& base) {
    if (name == "Base") {
        position[0] = base.position[0];
        position[1] = base.position[1];
    } else {
        throw std::runtime_error("A Unit can be deployed only on the home base's space.");
    }
}

// Check if the unit is a worker
bool Unit::isWorker() const {
    return name == "Worker";
}

void Unit::createUnit(const Unit& unit) {
    // Check if the current unit is a base
    if (name != "Base") {
        throw std::runtime_error("Only a base unit can create units.");
    }

    // Check if a unit is already being created
    if (currentCreation) {
        if (currentCreation->getId() != unit.getId() && currentCreation != nullptr) {
            throw std::runtime_error("Base is already creating a different unit.");
        }
        // Check if the current unit being created has finished building
        if (!currentCreation->buildingTick()) {
            return;  // Do nothing if the current unit is still being built
        }
        // Deploy the unit on the base itself
        currentCreation->deploy(*this);
        currentCreation = nullptr;  // Reset the currentCreation pointer
    } else {
        // Create a new unit
        currentCreation = new Unit(unit);
        // Check if the unit being created has already finished building
        if (!currentCreation->buildingTick()) {
            return;  // Do nothing if the unit is still being built
        }
        // Deploy the unit on the base itself
        currentCreation->deploy(*this);
        currentCreation = nullptr;  // Reset the currentCreation pointer
    }
}

// Check if a worker unit is on mine
bool Unit::isWorkerOnMine(const Map& map) const {
    // Check if the unit is a worker
    if (!isWorker()) {
        return false;
    }
    // Check if the unit's space is a mine space
    if (map.getCell(getPositionX(), getPositionY()) == '6') {
        return true;
    } else {
        return false;
    }
}