#ifndef BOARD_HPP
# define BOARD_HPP
# include <vector>
# include <string>
# include <iostream>

constexpr int SIZE = 19;

class Board {

	public:
		Board(void);
		Board(const Board& other);
		~Board();

		void display(void);
		const std::vector<std::vector<int>> getBoard(void) const;
		
	private: 
		std::vector<std::vector<int>> _board;

};

#endif