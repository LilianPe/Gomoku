#ifndef GAME_HPP
# define GAME_HPP
# include "Player.hpp"
# include "Board.hpp"
# include "Agent.hpp"
# include <string>
# include <memory>

// struct Move {
// 	int x, y;
// };

struct Capture {
	Move m1;
	Move m2;
	int index;
};

class Game{

	public:
		Game(std::shared_ptr<Board> Board);
		Game(std::shared_ptr<Board> Board, Player player1, Player player2);
		Game(const Game& other);
		~Game();

		void restart(void);
		void launch(void);
		void displayBoard(void);
		void nextTurn(void);
		bool moveIsValid(int x, int y, int p);
		void updateState(int x, int y);
		void resetCaptures(int n);

		int getCurrentTurn(void) const;
		Player& getPlayer1(void);
		Player& getPlayer2(void);
		Board& getBoard(void);
		std::vector<Capture>& getCaptured();
		void setBoard(Board& board);
		const Board& getBoard(void) const;
		void setEnd(bool end);
		bool getEnd(void) const;
		std::string getEndReason(void) const;
		int getWinnerId(void) const;
		void setWinnerId(int id);
		Player getWinner(void) const;
		Agent getAgent(void) const;
		Player getCurrentPlayer(void) const;
		std::vector<Move> getPlayerPawn(int x, int y);

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
		std::vector<Capture> _captured;
};

#endif