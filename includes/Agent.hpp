#ifndef AGENT_HPP
# define AGENT_HPP
# include <vector>
# include <string>
# include <iostream>
# include <algorithm>
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
		int evaluateBoard(Game& game, int id=0);
		int minimax(Game& game, int depth, bool isMaximizing, int alpha, int beta, int id=0, int currentScore=0);
		int _scoreMove(Game& game, int x, int y, int id);
		int _scoreLine(const int* line, int n, int playerId);
		int _computeLineScore(Game& game, int x, int y, int playerId, int enemyId);
		int _get_n_capturable(Game& game, int playerInd);
		int _getAllignmentFeatures(Game& game, int playerId);
		int _getAllignementValue(int size, int closed, bool hole);
		bool checkEnd(Game& game, int x, int y);
		bool _isInLimit(int x, int y);
		Game* _game;
};

#endif