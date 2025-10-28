#include "Game.hpp"

Game::Game(void) : _player1(Player()), _player2(Player()), _board(Board()) {}
Game::Game(Player player1, Player player2) : _player1(player1), _player2(player2), _board(Board()) {}
Game::Game(const Game& other) : _player1(other.getPlayer1()), _player2(other.getPlayer2()), _board(other.getBoard()) {}
Game::~Game() {}

void Game::displayBoard(void) {
	_board.display();
}

const Player Game::getPlayer1(void) const {
	return _player1;
}

const Player Game::getPlayer2(void) const {
	return _player2;
}

const Board Game::getBoard(void) const {
	return _board;
}
