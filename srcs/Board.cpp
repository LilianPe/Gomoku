#include "Board.hpp"

Board::Board(void) : _board(SIZE, std::vector<int>(SIZE, 0)) {}

Board::Board(const Board& other) : _board(other.getBoard()) {}

Board::~Board() {}

const std::vector<std::vector<int>> Board::getBoard(void) const {
	return _board;
}

void Board::display(void) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			std::cout << _board[i][j] << " ";
		}
		std::cout << std::endl;
	}
}
