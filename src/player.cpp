#include "player.hpp"

Player::Player(bool id, const std::string& name, unsigned int gold) : playerId(id), playerName(name), playerGold(gold) {
}

unsigned int Player::getGold() const {
    return playerGold;
}

bool Player::getID() const {
    return playerId;
}

std::string Player::getName() const {
    return playerName;
}

std::vector<Unit> Player::getPlayerUnits() {
    return playerUnits;
}

Unit Player::getUnitByID(unsigned short id) {
    for (auto &unit : playerUnits) {
        if (unit.getId() == id) {
            return unit;
        }
    }
    return playerUnits[0];
}

void Player::setGold(unsigned int amount) {
    playerGold = amount;
}

void Player::addUnitToPlayerUnits(const Unit& unit) {
    playerUnits.push_back(unit);
}
