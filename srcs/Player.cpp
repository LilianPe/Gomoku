#include "Player.hpp"

Player::Player(void) : _name("Player"), _score(0) {}
Player::Player(std::string name) : _name(name), _score(0) {}
Player::Player(std::string name, int score) : _name(name), _score(score) {}
Player::Player(const Player& other) : _name(other.getName()), _score(other.getScore()) {}
Player::~Player() {}

const std::string Player::getName() const {
	return _name;
}

int Player::getScore() const {
	return _score;
}
