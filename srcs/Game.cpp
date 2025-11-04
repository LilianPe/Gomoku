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
	_player1.resetCaptures();
	_player2.resetCaptures();
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
	for (int k = 0; k < SIZE; k++) {
		for (int j = 0; j < SIZE; j++) {
			if (_board.getCell(k, j) != 0) { // Optimization to check only occupied cells
				_checkFive(k, j);
			}
		}
	}
	for (int x2 = 0; x2 < SIZE; x2++) {
		for (int y2 = 0; y2 < SIZE; y2++) {
			_checkDoubleThree(x2, y2);
		}
	}
	if (!_end) {
		_checkCapture(x, y);
	}
}

void Game::_checkFive(int x, int y) {
	int cell = 0;
	int n = 0;
	int count = 0;
	std::vector<std::pair<int, int>> aligned_points;

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
			aligned_points.push_back({x + n * dx, y + n * dy});
			n++;
			count++;
		}
		n = -1;
		while (n > -5 && 
			_isInLimit(x + n * dx, y + n * dy) &&
			_board.getCell(x + n * dx, y + n * dy) == cell) {
			aligned_points.push_back({x + n * dx, y + n * dy});
			n--;
			count++;
		}
		if (count >= 5 && !_areCapturables(aligned_points)) {
			_end = true;
			_winner = cell;
			return;
		}
		aligned_points.clear();
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

void Game::_checkCapture(int x, int y) {
	int cell = _board.getCell(x, y);
	int opponentCell = 0;
	if (cell == 1) {
		opponentCell = 2;
	} else {
		opponentCell = 1;
	}
	std::vector<std::pair<int, int>> directions = {
		{1, 0},
		{1, 1},
		{0, 1},
		{-1, 1},
		{-1, 0},
		{-1, -1},
		{0, -1},
		{1, -1}
	};
	for (auto [dx, dy] : directions) {
		int x1 = x + dx;
		int y1 = y + dy;
		int x2 = x + 2 * dx;
		int y2 = y + 2 * dy;
		if (_isInLimit(x2, y2) &&
			_board.getCell(x1, y1) == opponentCell &&
			_board.getCell(x2, y2) == opponentCell) {
			int x3 = x + 3 * dx;
			int y3 = y + 3 * dy;
			if (_isInLimit(x3, y3) &&
				_board.getCell(x3, y3) == cell) {
				_board.setCell(x1, y1, 0);
				_board.setCell(x2, y2, 0);
				if (cell == 1) {
					_player1.incrementCaptures(2);
				} else {
				_player2.incrementCaptures(2);
				}
			}
		}
	}
	if (_player1.getCaptures() >= 10) {
		_end = true;
		_winner = 1;
		return ;
	}
	if (_player2.getCaptures() >= 10) {
		_end = true;
		_winner = 2;
		return ;
	}
}

bool Game::_areCapturables(const std::vector<std::pair<int, int>>& points) {
	for (const auto& [x, y] : points) {
		int cell = _board.getCell(x, y);
		int opponentCell = 0;
		if (cell == 1) {
			opponentCell = 2;
		} else {
			opponentCell = 1;
		}
		std::vector<std::pair<int, int>> directions = {
			{1, 0},
			{1, 1},
			{0, 1},
			{-1, 1},
			{-1, 0},
			{-1, -1},
			{0, -1},
			{1, -1}
		};
		for (auto [dx, dy] : directions) {
			int x1 = x + dx;
			int y1 = y + dy;
			
			if (!_isInLimit(x1, y1)) continue; // is in tab
			if (_board.getCell(x1, y1) != cell) continue; // has ally cell around

			int nxBefore = x - dx;
            int nyBefore = y - dy;
            int nxAfter  = x + 2 * dx;
            int nyAfter  = y + 2 * dy;

			if (!_isInLimit(nxBefore, nyBefore) || !_isInLimit(nxAfter, nyAfter)) continue;
			if (_board.getCell(nxBefore, nyBefore) == cell || _board.getCell(nxAfter, nyAfter) == cell) continue;

			return true; // Point is capturable
			}
	}
	return false;
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
