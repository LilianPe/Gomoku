# include "Agent.hpp"
# include "Player.hpp"

Agent::Agent(std::string name, Game game) : Player(name), _game(game) {}
Agent::Agent(std::string name, int score, int captures, Game game) : Player(name, score, captures), _game(game) {}
Agent::Agent(const Player& other, Game game) : Player(other), _game(game) {}
Agent::~Agent() {}

std::vector<Move> Agent::getAvailableMoves(Game& game) {
	std::vector<Move> moves;
	for (int y = 0; y < SIZE; y++) {
		for (int x = 0; x < SIZE; x++) {
			if (game.getBoard().getCell(x, y) == 0) {
				moves.push_back(Move{x, y});
			}
		}
	}
	return moves;
}

bool Agent::checkEnd(Game& game, int x, int y) {
	_game.updateState(x , y); // Placeholder implementation
	if (game.getEnd()) {
		return true;
	}
	return false;
}

int Agent::evaluateBoard(Game& game, int lastX, int lastY) {
    if (lastX != -1 && lastY != -1 && checkEnd(game, lastX, lastY)) {
        if (game.getBoard().getCell(lastX, lastY) == 1) return +10000;  // AI gagne
        if (game.getBoard().getCell(lastX, lastY) == 2) return -10000;  // Adversaire gagne
    }
    if (getAvailableMoves(game).empty()) return 0;  // Match nul

    // Heuristique simple (à remplacer par une vraie)
    int score = 0;
    // Exemple : +1 par pion AI, -1 par pion adversaire
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            int cell = game.getBoard().getCell(i, j);
            if (cell == 1) score += 1;
            if (cell == 2) score -= 1;
        }
    }
    return score;
}

int Agent::minimax(Game game, int depth, bool isMaximizing, int alpha, int beta, int lastX, int lastY) {
    // Note : on passe Game par VALEUR (copie) pour éviter undo

    bool hasLastMove = (lastX != -1 && lastY != -1);
    bool gameOver = hasLastMove && checkEnd(game, lastX, lastY);
    std::vector<Move> moves = getAvailableMoves(game);

    if (depth == 0 || gameOver || moves.empty()) {
        return evaluateBoard(game, lastX, lastY);
    }

    if (isMaximizing) {
        int maxEval = -2000000;
        for (const Move& move : moves) {
            Game temp = game;
            temp.getBoard().setCell(move.x, move.y, 1);
            int eval = minimax(temp, depth - 1, false, alpha, beta, move.x, move.y);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) break;
        }
        return maxEval;
    } else {
        int minEval = 2000000;
        for (const Move& move : moves) {
            Game temp = game;
            temp.getBoard().setCell(move.x, move.y, 2);
            int eval = minimax(temp, depth - 1, true, alpha, beta, move.x, move.y);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

// --- play() : version qui retourne le meilleur coup ---
std::pair<int, int> Agent::play() {
    std::vector<Move> moves = getAvailableMoves(_game);
    if (moves.empty()) return {-1, -1};  // plus de coups

    int bestScore = -2000000;
    Move bestMove = moves[0];

    const int depth = 8;  // Ajuste selon ton jeu

    for (const Move& move : moves) {
        Game temp = _game;
        temp.getBoard().setCell(move.x, move.y, 1);  // AI joue

        int score = minimax(temp, depth - 1, false,
                            -2000000, 2000000, move.x, move.y);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    // Appliquer le meilleur coup sur le vrai jeu
    // _game.getBoard().setCell(bestMove.x, bestMove.y, 1);

    return {bestMove.x, bestMove.y};
}
