#ifndef AGENT_HPP
# define AGENT_HPP
# include <vector>
# include <string>
# include <iostream> 
# include "Player.hpp"
# include "Board.hpp"
// # include "Game.hpp"

class Game;

struct Move;

struct Move {
	int x, y;
};

class Agent {

	public:
		Agent(Game& game);
		~Agent();
		std::pair<int, int> play(void);

		Game getGameCopy() const;
		Game& getGame() const;

	private:
		std::vector<Move> getAvailableMoves(Game& game);
		int evaluateBoard(Game& game , int lastX=-1, int lastY=-1);
		int minimax(Game game, int depth, bool isMaximizing, int alpha, int beta, int x=-1, int y=-1);
		bool checkEnd(Game& game, int x, int y);
		Game* _game;
};

#endif