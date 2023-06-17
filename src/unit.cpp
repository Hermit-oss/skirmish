#include "unit.hpp"

const std::unordered_map<std::string, UnitAttributes> Unit::unitAttributesMap = {
    {"Knight", {90, 5, 400, 1, 5}},
    {"Swordsman", {60, 2, 250, 5, 3}},
    {"Archer", {40, 2, 250, 5, 3}},
    {"Pikeman", {50, 2, 200, 2, 3}},
    {"Ram", {90, 2, 500, 1, 4}},
    {"Catapult", {50, 2, 800, 7, 6}},
    {"Worker", {20, 2, 100, 1, 2}},
    {"Base", {200, 0, 0, 0, 0}}
};

const std::unordered_map<std::string, std::unordered_map<std::string, unsigned short>> Unit::damageMap = {
    {"Knight", {{"Knight", 35}, {"Swordsman", 35}, {"Archer", 35}, {"Pikeman", 35}, {"Catapult", 35}, {"Ram", 50}, {"Worker", 35}, {"Base", 35}}},
    {"Swordsman", {{"Knight", 30}, {"Swordsman", 30}, {"Archer", 30}, {"Pikeman", 20}, {"Catapult", 20}, {"Ram", 30}, {"Worker", 30}, {"Base", 30}}},
    {"Archer", {{"Knight", 15}, {"Swordsman", 15}, {"Archer", 15}, {"Pikeman", 15}, {"Catapult", 10}, {"Ram", 10}, {"Worker", 15}, {"Base", 15}}},
    {"Pikeman", {{"Knight", 35}, {"Swordsman", 15}, {"Archer", 15}, {"Pikeman", 15}, {"Catapult", 15}, {"Ram", 10}, {"Worker", 15}, {"Base", 10}}},
    {"Catapult", {{"Knight", 40}, {"Swordsman", 40}, {"Archer", 40}, {"Pikeman", 40}, {"Catapult", 40}, {"Ram", 40}, {"Worker", 40}, {"Base", 50}}},
    {"Ram", {{"Knight", 10}, {"Swordsman", 10}, {"Archer", 10}, {"Pikeman", 10}, {"Catapult", 10}, {"Ram", 10}, {"Worker", 10}, {"Base", 50}}},
    {"Worker", {{"Knight", 5}, {"Swordsman", 5}, {"Archer", 5}, {"Pikeman", 5}, {"Catapult", 5}, {"Ram", 5}, {"Worker", 5}, {"Base", 1}}}
};

Unit::Unit(unsigned short id, const std::string& name) : id(id), name(name) {
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

// Getters

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

// Other member functions

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

unsigned short Unit::calculateDistance(unsigned short x, unsigned short y) {
    return std::abs(position[0] - x) + std::abs(position[1] - y);
}

void Unit::attackAction(unsigned short targetId, std::vector<Unit>& units) {
    if (speed == 0) {
        throw std::runtime_error("Unit cannot attack. Speed is 0.");
    }
    if (hasAttacked) {
        throw std::runtime_error("Unit can only attack once.");
    }
    
    // Find the target unit with the specified ID
    Unit* targetUnit = nullptr;
    for (Unit& unit : units) {
        if (unit.getId() == targetId) {
            targetUnit = &unit;
            break;
        }
    }

    if (targetUnit) {
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

void Unit::moveAction(unsigned short x, unsigned short y) {
    unsigned short distance = calculateDistance(x, y);

    if (distance > speed) {
        throw std::runtime_error("Movement distance exceeds the unit's speed.");
    }

    position[0] = x;
    position[1] = y;

    speed -= distance;
}

void Unit::takeDamage(unsigned short amount) {
    if (amount >= health) {
        health = 0;
    } else {
        health -= amount;
    }
}

bool Unit::buildingTick() {
    if (buildingTime > 0) {
        --buildingTime;
    }
    
    return buildingTime == 0;
}

void Unit::reset() {
    speed = baseSpeed;
    hasAttacked = false;
}

void Unit::deploy(const Unit& base) {
    if (name == "Base") {
        position[0] = base.position[0];
        position[1] = base.position[1];
    } else {
        throw std::runtime_error("A Unit can be deployed only on the home base's space.");
    }
}

bool Unit::isWorker() {
    if (name == "Worker") {
        return 1;
    } else {
        return 0;
    }
}
