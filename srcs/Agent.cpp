# include "Agent.hpp"
# include "Player.hpp"
# include "Game.hpp"

Agent::Agent(Game &game) : _game(&game) {}
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
    // game.getBoard().display();
	return moves;
}

bool Agent::checkEnd(Game& game, int x, int y) {
	getGameCopy().updateState(x , y); // Placeholder implementation
	if (game.getEnd()) {
		return true;
	}
	return false;
}

int Agent::evaluateBoard(Game& game, int lastX, int lastY) {
    if (lastX != -1 && lastY != -1 && checkEnd(game, lastX, lastY)) {
        if (game.getBoard().getCell(lastX, lastY) == 1) return + 1'000'000;  // AI gagne
        if (game.getBoard().getCell(lastX, lastY) == 2) return - 1'000'000;  // Adversaire gagne
    }
    if (getAvailableMoves(game).empty()) return 0;  // Match nul


    // Features (weight):
	// - allignement de 4 sans pion autour (100 000)
	// - allignement de 4 avec 1 pion qui bloque 1 cote (100)
	// - allignement de 3 avec 0 bloque (100)
	// - allignement de 3 avec 1 bloque (10)
	// - allignement de 2 avec 0 bloque (5)
	// - allignement de 2 avec 1 bloque (1)
	// - captures (300)
	// - pions capturables (100)
	// -- A ajouter potentiellement: 
	// -- Position de chaque pion par rapport au bord
	// --> pour chaque pion, feature += distance du bord le plus proche (weight -0.25-1)
	// -- Densite des pions (Insite a creer des groupe)
	// --> Genre pour chaque pion, si autour, il y a (XOX) -> +1 (weight 1-5) (prendre en compte que ce schemas sera compte 2 fois / occurence, diviser feature / 2) 
	// --- Inclure dans les alignements les schemas suivants:
	// 
	// 
	std::vector<int> features;
	std::vector<int> weights;

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
    std::vector<Move> moves = getAvailableMoves(getGame());
    if (moves.empty()) return {-1, -1};  // plus de coups
    int bestScore = -2000000;
    Move bestMove = moves[0];

    const int depth = 1;  // Ajuste selon ton jeu

    for (const Move& move : moves) {
        Game temp = getGameCopy();
        temp.getBoard().setCell(move.x, move.y, 1);  // AI joue

        int score = minimax(temp, depth - 1, false,
                            -2000000, 2000000, move.x, move.y);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    // Appliquer le meilleur coup sur le vrai jeu
    // getGameCopy().getBoard().setCell(bestMove.x, bestMove.y, 1);
    printf("x : %d, y : %d\n", bestMove.x, bestMove.y);
    return {bestMove.x, bestMove.y};
}


Game& Agent::getGame(void) const {
    return *_game;
}

Game Agent::getGameCopy(void) const {
    return *_game;
}
