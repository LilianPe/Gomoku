#ifndef DISPLAY_HPP
# define DISPLAY_HPP
# include "Board.hpp"
#include <SFML/Graphics.hpp>

class Display {

	public:
		Display(void);
		Display(Board board);
		Display(const Display& other);
		~Display();

		const Board getBoard(void) const;
		void open(void);
		
		private: 
		Board _board;
		const int _cellSize = 40;
		const int _gridSize = 19;
		
		void _drawGrid(sf::RenderWindow& window);
		void _drawPieces(sf::RenderWindow& window);

};

#endif