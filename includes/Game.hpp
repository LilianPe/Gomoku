#ifndef GAME_HPP
# define GAME_HPP
# include "Player.hpp"
# include "Board.hpp"
# include "Agent.hpp"
# include <string>
# include <memory>

class Game{

	public:
		Game(std::shared_ptr<Board> Board);
		Game(std::shared_ptr<Board> Board, Player player1, Player player2);
		Game(const Game& other);
		~Game();

		void restart(void);
		void displayBoard(void);
		void nextTurn(void);
		bool moveIsValid(int x, int y, int p);
		void updateState(int x, int y);

		int getCurrentTurn(void) const;
		Player& getPlayer1(void);
		Player& getPlayer2(void);
		Board& getBoard(void);
		const Board& getBoard(void) const;
		bool getEnd(void) const;
		std::string getEndReason(void) const;
		int getWinnerId(void) const;
		Player getWinner(void) const;
		Agent getAgent(void) const;
		Player getCurrentPlayer(void) const;

	private:
		void _checkFive(int x, int y);
		void _checkDoubleThree(int x, int y);
		void _checkCapture(int x, int y);
		bool _isInLimit(int x, int y);
		bool _areCapturables(const std::vector<std::pair<int, int>>& points);

		Player _player1;
		Player _player2;
		std::shared_ptr<Board> _board;
		int _currentTurn;
		int _winner;
		bool _end;
		std::string _endReason;
		Agent _agent;
};

#endif