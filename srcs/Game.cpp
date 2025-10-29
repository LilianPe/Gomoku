#include "Game.hpp"

Game::Game(Board& board) : _player1(Player()), _player2(Player()), _board(board), _currentTurn(1) {}

Game::Game(Board& board, Player player1, Player player2) : _player1(player1), _player2(player2), _board(board), _currentTurn(1) {}

// Game::Game(const Game& other) : _player1(other.getPlayer1()), _player2(other.getPlayer2()), _board(other.getBoard()), _currentTurn(other.getCurrentTurn()) {}

Game::~Game() {}


void Game::displayBoard(void) {
	_board.display();
}

void Game::nextTurn(void) {
	if (_currentTurn == 1) {
		_currentTurn = 2;
	} else {
		_currentTurn = 1;
	}
}

bool Game::moveIsValid(int x, int y, int p) {
	if (p != _currentTurn) {
		return false;
	}
	if (_board.getCell(x, y) != 0) {
		return false;
	}
	// Verifier les differentes regles du jeu
	return true;
}

int Game::getCurrentTurn(void) const {
	return _currentTurn;
}

const Player Game::getPlayer1(void) const {
	return _player1;
}

const Player Game::getPlayer2(void) const {
	return _player2;
}

Board& Game::getBoard(void) {
	return _board;
}

const Board& Game::getBoard(void) const {
	return _board;
}
