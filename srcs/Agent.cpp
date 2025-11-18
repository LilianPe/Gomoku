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
	game.updateState(x , y); // Placeholder implementation
	if (game.getEnd()) {
		return true;
	}
	return false;
}

int Agent::evaluateBoard(Game& game, int lastX, int lastY, int id) {
    int playerInd = id; // Definir player en fonction de quel player appelle l'agent
    int ennemyInd = (playerInd == 1) ? 2 : 1;
    if (lastX != -1 && lastY != -1 && checkEnd(game, lastX, lastY)) {
        if (game.getWinner().getId() == playerInd) return + 1'000'000;  // AI gagne
        else return (-1'000'000);  // Adversaire gagne
    }
    if (getAvailableMoves(game).empty()) return 0;  // Match nul


    // Features (weight):
	// - allignement de 4 sans pion autour (100 000) | X
	// - allignement de 4 avec 1 pion qui bloque 1 cote (100) | X
	// - allignement de 3 avec 0 bloque (100) | X
	// - allignement de 3 avec 1 bloque (10) | X
	// - allignement de 2 avec 0 bloque (5) | X
	// - allignement de 2 avec 1 bloque (1) | X
	// - peut etre prendre en compte les allignements de 5 et +, si un est mangeable | X
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
	std::vector<int> weights;

    int score = 0;
    std::vector<int> weight = {1, 300, 100};
    std::vector<int> playerFeatures = {0, 0, 0};
    std::vector<int> ennemyFeatures = {0, 0, 0};

    // set les features d'allignement
    playerFeatures[0] =  _getAllignmentFeatures(game,playerInd);
    ennemyFeatures[0] = _getAllignmentFeatures(game, ennemyInd);

    // captures
    Player player = (playerInd == 1) ? game.getPlayer1() : game.getPlayer2();
    Player ennemy = (ennemyInd == 1) ? game.getPlayer1() : game.getPlayer2();
    playerFeatures[1] = player.getCaptures();
    ennemyFeatures[1] = ennemy.getCaptures();

    //  possible captures
	playerFeatures[2] = _get_n_capturable(game, playerInd);
	ennemyFeatures[2] = _get_n_capturable(game, ennemyInd);

    // Calculating score
    for (long unsigned int i = 0; i < playerFeatures.size(); i++) {
        // printf("Feature %lu : player %d | ennemy %d\n", i, playerFeatures[i], ennemyFeatures[i]); 
        score += playerFeatures[i] * weight[i];
        score -= ennemyFeatures[i] * weight[i];
    }
    return score;
}

int Agent::_getAllignmentFeatures(Game& game, int playerId) {
    std::vector<std::pair<int, int>> directions = {
		{1, 0},
		{1, 1},
		{0, 1},
		{-1, 1}
	};
    int score = 0;
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            for (auto [dx, dy]: directions) {
                if ((game.getBoard().getCell(x, y) == playerId)
                    && ((!_isInLimit(x - dx, y - dy)) || (game.getBoard().getCell(x - dx, y - dy) != playerId))) {
                        // Ici, on est dans un allignement unique, plus qu'a trouver le type d'allignement, incrementer features[i] correspondant a cet allignement
                        int i = 1;
                        int size = 1;
                        int closed = 0;
                        bool hole = false;
                        int cell = 0;
                        if (!_isInLimit(x - dx, y - dy) || game.getBoard().getCell(x - dx, y - dy) != 0) {
                            closed++;
                        }
                        while (1) {
                            if (!_isInLimit(x + i * dx, y + i * dy)) {
                                closed++;
                                break ;
                            }
                            cell = game.getBoard().getCell(x + i * dx, y + i * dy);
                            if (cell != playerId) {
                                // Si hole == 0: hole = 1 et continur, sinon, break: plus de 2 trous, allignement finit, detecter si 2 trous d'affile, car allignement avec trou != sans trou
                                if (cell == 0) {
                                    if (!_isInLimit(x + (i + 1) * dx, y + (i + 1) * dy)) {
                                        break ;
                                    }
                                    int nextCell = game.getBoard().getCell(x + (i + 1) * dx, y + (i + 1) * dy);
                                    if (nextCell == 0) {
                                        break ;
                                    }
                                    else if (nextCell == playerId) {
                                        if (hole) {
                                            break ;
                                        }
                                        else {
                                            hole = true;
                                            i++;
                                            continue ;
                                        }
                                    }
                                    else {
                                        break ;
                                    }
                                }
                                else {
                                    closed++;
                                    break ;
                                }
                            }
                            else {
                                size++;
                            }
                            i++;
                        }
                        // Ici, on a toutes les infos necessaires, taille de l'allignement, si il comporte un trou, si il est ferme de 1 ou 2 cote ou pas, 
                        score += _getAllignementValue(size, closed, hole);
                }
            }
        }
    }
    return score;
}

int Agent::_getAllignementValue(int size, int closed, bool hole) {
    float closedWeights[3] = {1.0f, 0.01f, 0.0f};
    int sizePoints[5] = {0, 0, 5, 100, 100'000};

    float holeWeight = hole ? 1.2f : 1.0f;
    int sizePoint = size >= 5 ? 100'000 : sizePoints[size];

    return sizePoint * closedWeights[closed] * holeWeight;
}

int Agent::_get_n_capturable(Game& game, int playerInd) {
	std::vector<std::pair<int, int>> points;
	for (int x = 0; x < SIZE; x++){
		for (int y = 0; y < SIZE; y++) {
			if (game.getBoard().getCell(x, y) == playerInd)
				points.push_back({x, y});
		}
	}
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

int Agent::minimax(Game game, int depth, bool isMaximizing, int alpha, int beta, int lastX, int lastY, int id) {
    // Note : on passe Game par VALEUR (copie) pour éviter undo

    bool hasLastMove = (lastX != -1 && lastY != -1);
    bool gameOver = hasLastMove && checkEnd(game, lastX, lastY);
    std::vector<Move> moves = getAvailableMoves(game);
    int ennemyId = (id == 1) ? 2 : 1;

    if (depth == 0 || gameOver || moves.empty()) {
        return evaluateBoard(game, lastX, lastY, id);
    }

    if (isMaximizing) {
        int maxEval = -2000000;
        for (const Move& move : moves) {
            Game temp = game;
            temp.getBoard().setCell(move.x, move.y, id);
            int eval = minimax(temp, depth - 1, false, alpha, beta, move.x, move.y, id);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) break;
        }
        return maxEval;
    } else {
        int minEval = 2000000;
        for (const Move& move : moves) {
            Game temp = game;
            temp.getBoard().setCell(move.x, move.y, ennemyId);
            int eval = minimax(temp, depth - 1, true, alpha, beta, move.x, move.y, id);
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
        temp.getBoard().setCell(move.x, move.y, temp.getCurrentPlayer().getId());  // AI joue

        int score = minimax(temp, depth - 1, false,
                            -2000000, 2000000, move.x, move.y, temp.getCurrentPlayer().getId());
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    printf("x : %d, y : %d | score: %d\n", bestMove.x, bestMove.y, bestScore);
    return {bestMove.x, bestMove.y};
}


Game& Agent::getGame(void) const {
    return *_game;
}

Game Agent::getGameCopy(void) const {
    return *_game;
}
