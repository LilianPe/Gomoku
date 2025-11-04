#include "Player.hpp"

Player::Player(void) : _name("Player"), _score(0), _captures(0) {}
Player::Player(std::string name) : _name(name), _score(0), _captures(0) {}
Player::Player(std::string name, int score, int captures) : _name(name), _score(score), _captures(captures) {}
Player::Player(const Player& other) : _name(other.getName()), _score(other.getScore()), _captures(other.getCaptures()) {}
Player::~Player() {}

const std::string Player::getName() const {
	return _name;
}

int Player::getScore() const {
	return _score;
}

int Player::getCaptures() const {
	return _captures;
}

void Player::incrementCaptures(int n) {
	_captures += n;;
}

void Player::resetCaptures() {
	_captures = 0;
}
