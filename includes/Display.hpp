#ifndef DISPLAY_HPP
# define DISPLAY_HPP
# include "Board.hpp"
# include "Game.hpp"
// # include "Agent.hpp"
#include <sstream>
#include <string>
#include <thread>
#include <SFML/Graphics.hpp>

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

class Display {

	public:
		Display(void);
		Display(Board board);
		Display(const Display& other);
		~Display();

		Board& getBoard(void);
		const Board& getBoard(void) const;
		const Game getGame(void) const;
		GameState getState(void) const;
		void open(void);
		
		private:
		std::shared_ptr<Board>  _board;
		Game _game;
		GameState _state;
		const int _cellSize = 40;
		sf::Time _whiteFrozenTurnClock;
		sf::Time _blackFrozenTurnClock;
		sf::Clock _whiteTurnClock;
		sf::Clock _blackTurnClock;
		sf::Clock _aiClock;
		bool _waitingForAi;
		sf::CircleShape _currentSuggestion;
		const int _gridSize = SIZE;
		
		void _drawGrid(sf::RenderWindow& window);
		void _drawPieces(sf::RenderWindow& window);
		void _drawScores(sf::RenderWindow& window, int windowSize, sf::Font& Font);
		void _handleMove(sf::Event& event);
		void _handleMenu(sf::Event& event, sf::RenderWindow& window, int windowSize);
		void _handleButtons(sf::RenderWindow& window, sf::Event& event, int windowSize);
		void _displayMenu(sf::RenderWindow& window, sf::Font& font, int windowSize);
		void _displayEndScreen(sf::RenderWindow& window, sf::Font& font, int windowSize);
		void _playMove(int x, int y, int player);
		void _updateBoard(sf::RenderWindow& window, int windowSize, sf::Font& font);
		void _handleEvents(sf::RenderWindow& window, int windowSize);
		void _displayTimer(sf::RenderWindow& window, sf::Font& font, int x, int y, int id);
		void _displayShadow(sf::RenderWindow& window);
		void _displayEndMessage(sf::RenderWindow& window, sf::Font& font, int windowSize);
		void _drawReplayButton(sf::RenderWindow& window, sf::Font& font, int windowSize);
		void _drawLeavesButton(sf::RenderWindow& window, sf::Font& font, int windowSize);
		void _updateSuggestion();
		// void _processAiTurn(Game &game);
};

#endif