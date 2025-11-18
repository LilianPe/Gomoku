#include "Player.hpp"

Player::Player(void) : _name("Player"), _score(0), _captures(0), _id(0) {}
Player::Player(std::string name, std::string type) : _name(name), _type(type), _score(0), _captures(0), _id(0) {}
Player::Player(std::string name, int score, int captures) : _name(name), _score(score), _captures(captures), _id(0) {}
Player::Player(const Player& other) : _name(other.getName()), _type(other.getType()), _score(other.getScore()), _captures(other.getCaptures()), _id(other.getId()) {}
Player::~Player() {}

const std::string Player::getName() const {
	return _name;
}

const std::string Player::getType() const {
	return _type;
}

void Player::setType(std::string newType) {
	printf("Type modified ! NewType : %s\n", newType.c_str());
	_type = newType;
}

int Player::getScore() const {
	return _score;
}

void Player::setId(int newId) {
	_id = newId;
}

int Player::getId() const {
	return _id;
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
