#include "Board.hpp"

Board::Board(void) : _board(SIZE, std::vector<int>(SIZE, 0)) {}

Board::Board(const Board& other) : _board(other.getBoard()) {}

Board::~Board() {}

void Board::display(void) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			std::cout << _board[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void Board::clear(void) {
	_board = std::vector<std::vector<int>>(SIZE, std::vector<int>(SIZE, 0));
}

int Board::getCell(int x, int y) {
	return _board[y][x];
}

void Board::setCell(int x, int y, int value) {
	_board[y][x] = value;
}

const std::vector<std::vector<int>> Board::getBoard(void) const {
	return _board;
}