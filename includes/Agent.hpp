#ifndef AGENT_HPP
# define AGENT_HPP
# include <vector>
# include <unordered_map>
# include <string>
# include <iostream>
# include <cstdint>
# include "Player.hpp"
# include "Board.hpp"
# include <chrono>
// # include "Game.hpp"

class Game;

struct Move;

struct Move {
	int x, y;
};

enum class TTFlag: uint8_t {
	EXACT = 0,
	LOWERBOUND = 1,
	UPPERBOUND = 2
};

struct TTValue {
	int score;
	int depth;
	TTFlag flag;
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
		int evaluateBoard(Game& game , int lastX=-1, int lastY=-1, int id=0);
		int minimax(Game& game, int depth, bool isMaximizing, int alpha, int beta, int x=-1, int y=-1, int id=0);
		int _get_n_capturable(Game& game, int playerInd);
		int _getAllignmentFeatures(Game& game, int playerId);
		int _getAllignementValue(int size, int closed, bool hole);
		bool checkEnd(Game& game, int x, int y);
		bool _isInLimit(int x, int y);
		void _updateTable(Game& game, TTFlag flag, int score, int depth, uint64_t hash);
		void _restoreGameValue(Game& game, Move& move, int origPlayer1Captures, int origPlayer2Captures);
		std::unordered_map<uint64_t, TTValue> _transpoTable;
		std::unordered_map<uint64_t, int> _boardValues;
		std::unordered_map<uint64_t, std::vector<Move>> _boardMoves;
		Game* _game;
};

#endif