#ifndef DISPLAY_HPP
# define DISPLAY_HPP
# include "Board.hpp"
# include "Game.hpp"
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>

class Display {

	public:
		Display(void);
		Display(Board board);
		Display(const Display& other);
		~Display();

		const Board getBoard(void) const;
		const Game getGame(void) const;
		void open(void);
		
		private: 
		Board _board;
		Game _game;
		const int _cellSize = 40;
		const int _gridSize = SIZE;
		
		void _drawGrid(sf::RenderWindow& window);
		void _drawPieces(sf::RenderWindow& window);

};

#endif