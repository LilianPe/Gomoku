#ifndef BOARD_HPP
# define BOARD_HPP
# include <vector>
# include <string>
# include <iostream>
# include <cstdint>

constexpr int SIZE = 19;

class Board {

	public:
		Board(void);
		Board(const Board& other);
		Board& operator=(const Board& other); 
		~Board();

		void display(void);
		void clear(void);
		int getCell(int x, int y) const;
		void setCell(int x, int y, int value);
		const std::vector<std::vector<int>> getBoard(void) const;
		uint64_t getBoardHash(void);
		
	private: 
		std::vector<std::vector<int>> _board;

};

#endif