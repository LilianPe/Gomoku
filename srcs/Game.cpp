#include "Game.hpp"

Game::Game(Board& board) :
	_player1(Player()),
	_player2(Player()),
	_board(board),
	_currentTurn(1),
	_winner(0),
	_end(false) {}

Game::Game(Board& board, Player player1, Player player2) : 
	_player1(player1),
	_player2(player2),
	_board(board),
	_currentTurn(1),
	_winner(0),
	_end(false) {}

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
	return !_end;
}

void Game::updateState(void) {
	_checkRow();
	_checkCollumn();
}

void Game::_checkRow(void) {
	int cell = 0;
	for (int i = 0; i < SIZE; i++) {
		int prevCell = 0;
		int aligned = 0;
		for (int j = 0; j < SIZE; j++) {
			cell = _board.getCell(j, i);
			if (cell != 0 && cell == prevCell) {
				aligned++;
			}
			else if (cell != 0) {
				aligned = 1;
			}
			else {
				aligned = 0;
			}
			if (aligned == 5) {
				_end = true;
				_winner = cell;
			}
			prevCell = cell;
		}
	}
}

void Game::_checkCollumn(void) {
	int cell = 0;
	for (int i = 0; i < SIZE; i++) {
		int prevCell = 0;
		int aligned = 0;
		for (int j = 0; j < SIZE; j++) {
			cell = _board.getCell(i, j);
			if (cell != 0 && cell == prevCell) {
				aligned++;
			}
			else if (cell != 0) {
				aligned = 1;
			}
			else {
				aligned = 0;
			}
			if (aligned == 5) {
				_end = true;
				_winner = cell;
			}
			prevCell = cell;
		}
	}
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

bool Game::getEnd(void) {
	return _end;
}

int Game::getWinner(void) {
	return _winner;
}
