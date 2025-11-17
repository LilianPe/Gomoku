# include "Agent.hpp"
# include "Player.hpp"
# include "Game.hpp"

Agent::Agent(Game &game) : _game(&game) {}
Agent::~Agent() {}


// --- getAvailableMoves() : retourne la liste des coups possibles(ne prend que les coups proches de pions existants pour optimiser) ---
std::vector<Move> Agent::getAvailableMoves(Game& game) {
    std::vector<Move> moves;

    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {

            if (game.getBoard().getCell(x, y) != 0) continue;

            bool relevant = false;

            for (int dy = -1; dy <= 1 && !relevant; dy++) {
                for (int dx = -1; dx <= 1 && !relevant; dx++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx < 0 || ny < 0 || nx >= SIZE || ny >= SIZE) continue;
                    if (game.getBoard().getCell(nx, ny) != 0)
                        relevant = true;
                }
            }

            if (relevant)
                moves.push_back({x, y});
        }
    }
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
    int player_ind = 1; // Definir player en fonction de quel player appelle l'agent
    if (lastX != -1 && lastY != -1 && checkEnd(game, lastX, lastY)) {
        if (game.getBoard().getCell(lastX, lastY) == player_ind) return + 1'000'000;  // AI gagne
        else return - 1'000'000;  // Adversaire gagne
    }
    if (getAvailableMoves(game).empty()) return 0;  // Match nul


    // Features (weight):
	// - allignement de 4 sans pion autour (100 000) | X
	// - allignement de 4 avec 1 pion qui bloque 1 cote (100) | X
	// - allignement de 3 avec 0 bloque (100) | X
	// - allignement de 3 avec 1 bloque (10) | X
	// - allignement de 2 avec 0 bloque (5) | X
	// - allignement de 2 avec 1 bloque (1) | X
	// - captures (300) | V
	// - pions capturables (100) | V
    //
    //
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
    std::vector<int> weight = {100'000, 100, 100, 10, 5, 1, 300, 100};
    std::vector<int> features = {0, 0, 0, 0, 0, 0, 0, 0};

    // set les features d'allignement

    // captures:
    Player player = (player_ind == 1) ? game.getPlayer1() : game.getPlayer2();
    features[6] = player.getCaptures();

    // captures possibles
    std::vector<std::pair<int, int>> points;
	for (int x = 0; x < SIZE; x++){
		for (int y = 0; y < SIZE; y++) {
			if (game.getBoard().getCell(x, y) == player_ind)
				points.push_back({x, y});
		}
	}
	features[7] = _get_n_capturable(game, points);
    for (int i = 0; i < features.size(); i++) {
        score += features[i] * weight[i];
    }
    return score;
}

int Agent::_get_n_capturable(Game& game, const std::vector<std::pair<int, int>>& points) {
	int captures = 0;
    for (const auto& [x, y] : points) {
		int cell = game.getBoard().getCell(x, y);
		std::vector<std::pair<int, int>> directions = {
			{1, 0},
			{1, 1},
			{0, 1},
			{-1, 1},
			{-1, 0},
			{-1, -1},
			{0, -1},
			{1, -1}
		};
		for (auto [dx, dy] : directions) {
			int x1 = x + dx;
			int y1 = y + dy;
			int nxBefore = x - dx;
            int nyBefore = y - dy;
            int nxAfter  = x + 2 * dx;
            int nyAfter  = y + 2 * dy;
			if (!_isInLimit(x1, y1)) continue; // is in tab
			if (game.getBoard().getCell(x1, y1) != cell) continue; // has ally cell around

	
			if (!_isInLimit(nxBefore, nyBefore) || !_isInLimit(nxAfter, nyAfter)) continue;
			if (game.getBoard().getCell(nxBefore, nyBefore) == cell || game.getBoard().getCell(nxAfter, nyAfter) == cell) continue;
			if (game.getBoard().getCell(nxBefore, nyBefore) == 0) {
				if (game.getBoard().getCell(nxAfter, nyAfter) == 0) {
					continue;
				}
			}
			captures++; // Point is capturable
			}
	}
	return captures;
}

bool Agent::_isInLimit(int x, int y) {
	return (x >= 0 && x < SIZE && y >= 0 && y < SIZE);
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

    const int depth = 2;  // Ajuste selon ton jeu

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
    printf("x : %d, y : %d\n", bestMove.x, bestMove.y);
    return {bestMove.x, bestMove.y};
}


Game& Agent::getGame(void) const {
    return *_game;
}

Game Agent::getGameCopy(void) const {
    return *_game;
}
