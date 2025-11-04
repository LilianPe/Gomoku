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
		void _drawScores(sf::RenderWindow& window, int windowSize, sf::Font& Font);
		void _handleButtons(sf::RenderWindow& window, sf::Event& event, int windowSize);
		void _displayEndScreen(sf::RenderWindow& window, sf::Font& font, int windowSize);
		void _playMove(int x, int y, int player);
		void _updateBoard(sf::RenderWindow& window, int windowSize, sf::Font& font);
		void _handleEvents(sf::RenderWindow& window, int windowSize);
		void _displayShadow(sf::RenderWindow& window);
		void _displayEndMessage(sf::RenderWindow& window, sf::Font& font, int windowSize);
		void _drawReplayButton(sf::RenderWindow& window, sf::Font& font, int windowSize);
		void _drawLeavesButton(sf::RenderWindow& window, sf::Font& font, int windowSize);
};

#endif