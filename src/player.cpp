#include "player.hpp"

Player::Player(bool id, const std::string& name) : playerId(id), playerName(name) {}

unsigned int Player::getGold() const {
    return gold;
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

void Player::setGold(unsigned int amount) {
    gold = amount;
}

void Player::addUnitToPlayerUnits(const Unit& unit) {
    playerUnits.push_back(unit);
}
