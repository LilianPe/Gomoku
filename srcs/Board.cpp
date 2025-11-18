#include "Board.hpp"

Board::Board(void) : _board(SIZE, std::vector<int>(SIZE, 0)) {}

Board::Board(const Board& other) : _board(SIZE, std::vector<int>(SIZE, 0)){
	for (int y = 0; y < SIZE; y++) {
		for (int x = 0; x < SIZE; x++) {
			_board[y][x] = other.getCell(x, y);
		}
	}
}

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

int Board::getCell(int x, int y) const {
	return _board[y][x];
}

void Board::setCell(int x, int y, int value) {
	_board[y][x] = value;
}

const std::vector<std::vector<int>> Board::getBoard(void) const {
	return _board;
}

uint64_t Board::getBoardHash(void) {
    std::string s;
    s.reserve(361);
    for (int y = 0; y < SIZE; y++)
        for (int x = 0; x < SIZE; x++)
            s += char(getCell(x, y));  // 0,1,2 → caractères différents
    return std::hash<std::string>{}(s);
}