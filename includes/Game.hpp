#ifndef GAME_HPP
# define GAME_HPP
# include "Player.hpp"
# include "Board.hpp"

class Game{

	public:
		Game(void);
		Game(Player player1, Player player2);
		Game(const Game& other);
		~Game();

		void displayBoard(void);
		const Player getPlayer1(void) const ;
		const Player getPlayer2(void) const ;
		const Board getBoard(void) const ;
	
	private:
		Player _player1;
		Player _player2;
		Board _board;
};

#endif