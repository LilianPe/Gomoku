#ifndef GAME_HPP
# define GAME_HPP
# include "Player.hpp"
# include "Board.hpp"

class Game{

	public:
		Game(Board& board);
		Game(Board& board, Player player1, Player player2);
		// Game(const Game& other);
		~Game();

		void restart(void);
		void displayBoard(void);
		void nextTurn(void);
		bool moveIsValid(int x, int y, int p);
		void updateState(int x, int y);

		int getCurrentTurn(void) const;
		const Player getPlayer1(void) const;
		const Player getPlayer2(void) const;
		Board& getBoard(void);
		const Board& getBoard(void) const;
		bool getEnd(void);
		Player getWinner(void);
	
	private:
		void _checkFive(int x, int y);
		void _checkDoubleThree(int x, int y);
		void _checkCapture(int x, int y);
		bool _isInLimit(int x, int y);
		bool _areCapturables(const std::vector<std::pair<int, int>>& points);

		Player _player1;
		Player _player2;
		Board& _board;
		int _currentTurn;
		int _winner;
		bool _end;
};

#endif