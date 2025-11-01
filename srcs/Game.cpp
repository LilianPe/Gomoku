#include "Game.hpp"

Game::Game(Board& board) :
	_player1(Player("Player1")),
	_player2(Player("Player2")),
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

void Game::restart(void) {
	_board.clear();
	_currentTurn = 1;
	_winner = 0;
	_end = false;
}

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

void Game::updateState(int x, int y) {
	_checkFive(x, y);
	for (int x2 = 0; x2 < SIZE; x2++) {
		for (int y2 = 0; y2 < SIZE; y2++) {
			_checkDoubleThree(x2, y2);
		}
	}
}

void Game::_checkFive(int x, int y) {
	int cell = 0;
	int n = 0;
	int count = 0;
	std::vector<std::pair<int, int>> directions = {
		{1, 0},
		{1, 1},
		{0, 1},
		{-1, 1}
	};
	for (auto [dx, dy] : directions) {
		cell = _board.getCell(x, y);
		n = 1;
		count = 1;
		while (n < 5 &&
			_isInLimit(x + n * dx, y + n * dy) &&
			_board.getCell(x + n * dx, y + n * dy) == cell) {
			n++;
			count++;
		}
		n = -1;
		while (n < 5 && 
			_isInLimit(x + n * dx, y + n * dy) &&
			_board.getCell(x + n * dx, y + n * dy) == cell) {
			n--;
			count++;
		}
		if (count >= 5) {
			_end = true;
			_winner = cell;
			return ;
		}
	}
}

void Game::_checkDoubleThree(int x, int y) {
	int pawnCell = 0;
	int cell = 0;
	int n = 0;
	int count = 0;
	int threes = 0;
	int emptyFirst = 0;
	int emptySecond = 0;
	int emptyMid = 0;
	std::vector<std::pair<int, int>> directions = {
		{1, 0},
		{1, 1},
		{0, 1},
		{-1, 1}
	};
	for (auto [dx, dy] : directions) {
		pawnCell = _board.getCell(x, y);
		n = 1;
		count = 1;
		emptyFirst = 0;
		emptySecond = 0;
		emptyMid = 0;
		while (n < 5 && _isInLimit(x + n * dx, y + n * dy)) {
			cell = _board.getCell(x + n * dx, y + n * dy);
			if (cell == pawnCell) {
				count++;
			}
			else if (cell == 0) {
				if (!emptyMid) {
					emptyMid = 1;
				}
				else if (emptyMid == 1 && _board.getCell(x + (n - 1) * dx, y + (n - 1) * dy) == 0) {
					emptyMid = 0;
					emptyFirst = 1;
					break;
				}
				else if (emptyMid == 1 && _board.getCell(x + (n - 1) * dx, y + (n - 1) * dy) == pawnCell) {
					emptyFirst = 1;
					break;
				}
			}
			else {
				break;
			}
			n++;
		}
		n = -1;
		while (abs(n) < 5 && _isInLimit(x + n * dx, y + n * dy)) {
			cell = _board.getCell(x + n * dx, y + n * dy);
			if (cell == pawnCell) {
				count++;
			}
			else if (cell == 0) {
				if (!emptyMid) {
					emptyMid = 1;
				}
				else if (emptyMid == 1 && _board.getCell(x + (n + 1) * dx, y + (n + 1) * dy) == 0) {
					emptyMid = 0;
					emptySecond = 1;
					break;
				}
				else if (emptyMid == 1 && _board.getCell(x + (n + 1) * dx, y + (n + 1) * dy) == pawnCell) {
					emptySecond = 1;
					break;
				}
			}
			else {
				break;
			}
			n--;
		}
		if (count == 3 && emptyFirst == 1 && emptySecond == 1) {
			threes++;
		}
	}
	if (threes >= 2) {
		_end = true;
		_winner = (pawnCell == 1) ? 2 : 1;
		return ;
	}
}

bool Game::_isInLimit(int x, int y) {
	return (x >= 0 && x < SIZE && y >= 0 && y < SIZE);
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

Player Game::getWinner(void) {
	if (_winner == 1) return _player1;
	else if (_winner == 2) return _player2;
	else throw std::logic_error("No winner!");
}
