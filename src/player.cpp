#include "player.hpp"

Player::Player(bool id, const std::string& name) : playerId(id), playerName(name) {
    playerUnits.push_back(Unit(playerId, playerId, "Base"));
}

bool Player::getID() const {
    return playerId;
}

std::string Player::getName() const {
    return playerName;
}

std::vector<Unit> Player::getPlayerUnits() const {
    return playerUnits;
}

void Player::addUnitToPlayerUnits(const Unit& unit) {
    playerUnits.push_back(unit);
}
