# include "Agent.hpp"
# include "Player.hpp"
# include "Game.hpp"

Agent::Agent(Game &game) : _game(&game) {
    // _transpoTable.reserve(5'000'000);
}
Agent::~Agent() {}


// --- getAvailableMoves() : retourne la liste des coups possibles(ne prend que les coups proches de pions existants pour optimiser) ---
std::vector<Move> Agent::getAvailableMoves(Game& game) {
    std::vector<Move> moves;
    bool empty = true;

    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {

            if (game.getBoard().getCell(x, y) != 0) {
                empty = false;
                continue;
            }

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
    if (empty) {
        moves.push_back({9, 9});
    }
    return moves;
}

bool Agent::checkEnd(Game& game, int x, int y) {
	game.updateState(x , y); // Placeholder implementation
    // Ne pas appeller updateState, faire une fonction beaucoup plus rapide qui verifie uniquement les coups autour de x / y
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
    std::vector<int> weight = {1, 100};
    std::vector<int> playerFeatures = {0, 0};
    std::vector<int> ennemyFeatures = {0, 0};

    // set les features d'allignement
    playerFeatures[0] =  _getAllignmentFeatures(game,playerInd);
    ennemyFeatures[0] = _getAllignmentFeatures(game, ennemyInd);

    //  possible captures
	playerFeatures[1] = _get_n_capturable(game, playerInd);
	ennemyFeatures[1] = _get_n_capturable(game, ennemyInd);

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

int Agent::minimax(Game& game, int depth, bool isMaximizing, int alpha, int beta, int lastX, int lastY, int id) {
    // Note : on passe Game par VALEUR (copie) pour éviter undo
    uint64_t boardKey = game.getBoard().getBoardHash();
    Player player = (id == 1) ? game.getPlayer1() : game.getPlayer2();
    Player ennemy = (id == 1) ? game.getPlayer2() : game.getPlayer1();
    int playerCaptures = player.getCaptures();
    int ennemyCaptures = ennemy.getCaptures();
    auto it = _transpoTable.find(boardKey);
    if (it != _transpoTable.end() && it->second.depth >= depth) {
        auto& entry = it->second;
        if (entry.flag == TTFlag::EXACT) {
            // std::cout << "User TT!! key : " << boardKey << std::endl;
            return it->second.score + playerCaptures - ennemyCaptures;
        }
        else if (entry.flag == TTFlag::LOWERBOUND && entry.score >= beta) {
            // std::cout << "User TT!! key : " << boardKey << std::endl;
            return it->second.score + playerCaptures - ennemyCaptures;
        }
        else if (entry.flag == TTFlag::UPPERBOUND && entry.score <= alpha) {
            // std::cout << "User TT!! key : " << boardKey << std::endl;
            return it->second.score + playerCaptures - ennemyCaptures;
        }
    }

    bool hasLastMove = (lastX != -1 && lastY != -1);
    bool gameOver = hasLastMove && checkEnd(game, lastX, lastY);
    auto it2 = _boardMoves.find(boardKey);
    std::vector<Move> moves;
    if (it2 != _boardMoves.end()) {
        moves = it2->second;
    }
    else {
        moves = getAvailableMoves(game);
        _boardMoves[boardKey] = moves;
    }
    int ennemyId = (id == 1) ? 2 : 1;

    if (depth == 0 || gameOver || moves.empty()) {
        int score = 0;
        auto it3 = _boardValues.find(boardKey);
        if (it3 != _boardValues.end()) {
            score = it3->second;
        }
        else {
            score = evaluateBoard(game, lastX, lastY, id);
            _boardValues[boardKey] = score;
        }
        _updateTable(game, TTFlag::EXACT, score, depth, boardKey);
        return score + playerCaptures - ennemyCaptures;
    }

    int origAlpha = alpha;
    int origPlayer1Captures = game.getPlayer1().getCaptures();
    int origPlayer2Captures = game.getPlayer2().getCaptures();
    // Board origBoard = Board(game.getBoard());
    if (isMaximizing) {
        int maxEval = -2000000;
        for (Move& move : moves) {
            game.getBoard().setCell(move.x, move.y, id);
            int eval = minimax(game, depth - 1, false, alpha, beta, move.x, move.y, id);
            // game.getBoard().setCell(move.x, move.y, 0);
            _restoreGameValue(game, move, origPlayer1Captures, origPlayer2Captures);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break ;
            };
        }
        TTFlag flag = (maxEval <= origAlpha) ? TTFlag::UPPERBOUND : (maxEval >= beta) ? TTFlag::LOWERBOUND : TTFlag::EXACT;
        _updateTable(game, flag, maxEval, depth, boardKey);
        return maxEval;
    } else {
        int minEval = 2000000;
        for (Move& move : moves) {
            game.getBoard().setCell(move.x, move.y, ennemyId);
            int eval = minimax(game, depth - 1, true, alpha, beta, move.x, move.y, id);
            // game.getBoard().setCell(move.x, move.y, 0);
            _restoreGameValue(game, move, origPlayer1Captures, origPlayer2Captures);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break;
            }
        }
        TTFlag flag = (minEval <= origAlpha) ? TTFlag::UPPERBOUND : (minEval >= beta) ? TTFlag::LOWERBOUND : TTFlag::EXACT;
        _updateTable(game, flag, minEval, depth, boardKey);
        return minEval;
    }
}

void Agent::_restoreGameValue(Game& game, Move& move, int origPlayer1Captures, int origPlayer2Captures) {
    int captures = game.getPlayer1().getCaptures() + game.getPlayer2().getCaptures() - origPlayer1Captures - origPlayer2Captures;
    game.setEnd(false);
    game.getPlayer1().setCaptures(origPlayer1Captures);
    game.getPlayer2().setCaptures(origPlayer2Captures);
    game.getBoard().setCell(move.x, move.y, 0);
    game.setWinnerId(0);
    if (captures) {
        game.resetCaptures(1);
    }
}

void Agent::_updateTable(Game& game, TTFlag flag, int score, int depth, uint64_t hash) {
    if (!hash) {
        hash = game.getBoard().getBoardHash();
    }
    auto it = _transpoTable.find(hash);
    if (it != _transpoTable.end() && it->second.depth > depth) {
        return ;
    }
    TTValue value;
    value.score = score;
    value.depth = depth;
    value.flag = flag;
    _transpoTable[hash] = value;
}

// --- play() : version qui retourne le meilleur coup ---
std::pair<int, int> Agent::play() {
    std::vector<Move> moves = getAvailableMoves(getGame());
    if (moves.empty()) return {-1, -1};  // plus de coups
    if (moves.size() == 1) return {moves[0].x, moves[0].y};
    int bestScore = -2000000;
    Move bestMove = moves[0];
    
    
    int currentPlayer = getGame().getCurrentPlayer().getId();
    Game temp = getGameCopy();
    int origPlayer1Captures = temp.getPlayer1().getCaptures();
    int origPlayer2Captures = temp.getPlayer2().getCaptures();
    // Board origBoard = Board(temp.getBoard());
    // auto start = std::chrono::high_resolution_clock::now();
    
    int depth;
    for (depth = 1; depth <= 2; ++depth) {
        if (depth > 1) {
            temp.getBoard().setCell(bestMove.x, bestMove.y, currentPlayer);
            int score = minimax(temp, depth - 1, false, -2000000, 2000000,
                            bestMove.x, bestMove.y, currentPlayer);
            _restoreGameValue(temp, bestMove, origPlayer1Captures, origPlayer2Captures);
            bestScore = score;
        }
        for (Move& move : moves) {
            if (depth > 1 && move.x == bestMove.x && move.y == bestMove.y)
                continue;
            temp.getBoard().setCell(move.x, move.y, currentPlayer);  // AI joue
            int score = minimax(temp, depth - 1, false,
                -2000000, 2000000, move.x, move.y, currentPlayer);
            _restoreGameValue(temp, move, origPlayer1Captures, origPlayer2Captures);
            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
            // auto now = std::chrono::high_resolution_clock::now();
            // if (std::chrono::duration_cast<std::chrono::milliseconds>(now-start).count() > 800) // 800ms max
            //     break;
            }
            // auto now = std::chrono::high_resolution_clock::now();
            // if (std::chrono::duration_cast<std::chrono::milliseconds>(now-start).count() > 800) // 800ms max
            //     break;
    }
    // for (Move& move : moves) {
    //     temp.getBoard().setCell(move.x, move.y, currentPlayer);  // AI joue
    //     int score = minimax(temp, depth - 1, false,
    //         -2000000, 2000000, move.x, move.y, currentPlayer);
    //     _restoreGameValue(temp, move, origPlayer1Captures, origPlayer2Captures);
    //     if (score > bestScore) {
    //         bestScore = score;
    //         bestMove = move;
    //     }
    // }
    printf("x : %d, y : %d | score: %d | depth: %d\n", bestMove.x, bestMove.y, bestScore, depth);
    return {bestMove.x, bestMove.y};
}

Game& Agent::getGame(void) const {
    return *_game;
}

Game Agent::getGameCopy(void) const {
    return *_game;
}
