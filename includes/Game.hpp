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

		void displayBoard(void);
		void nextTurn(void);
		bool moveIsValid(int x, int y, int p);

		int getCurrentTurn(void) const;
		const Player getPlayer1(void) const;
		const Player getPlayer2(void) const;
		Board& getBoard(void);
		const Board& getBoard(void) const;
	
	private:
		Player _player1;
		Player _player2;
		Board& _board;
		int _currentTurn;
};

#endif