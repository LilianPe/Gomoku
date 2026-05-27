# include "Agent.hpp"
# include "Player.hpp"
# include "Game.hpp"

Agent::Agent(Game &game) : _game(&game) {}
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
	if (game.getEnd()) {
		return true;
	}
	return false;
}

int Agent::evaluateBoard(Game& game, int id) {
    int playerInd = id;
    int ennemyInd = (playerInd == 1) ? 2 : 1;
    if (game.getEnd()) {
        if (game.getWinnerId() == playerInd) return +1'000'000;
        return -1'000'000;
    }

    int score = 0;
    std::vector<int> weight = {1, 300, 100};
    std::vector<int> playerFeatures = {0, 0, 0};
    std::vector<int> ennemyFeatures = {0, 0, 0};

    playerFeatures[0] = _getAllignmentFeatures(game, playerInd);
    ennemyFeatures[0] = _getAllignmentFeatures(game, ennemyInd);

    Player player = (playerInd == 1) ? game.getPlayer1() : game.getPlayer2();
    Player ennemy = (ennemyInd == 1) ? game.getPlayer1() : game.getPlayer2();
    playerFeatures[1] = player.getCaptures();
    ennemyFeatures[1] = ennemy.getCaptures();

    playerFeatures[2] = _get_n_capturable(game, playerInd);
    ennemyFeatures[2] = _get_n_capturable(game, ennemyInd);

    for (long unsigned int i = 0; i < playerFeatures.size(); i++) {
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

int Agent::minimax(Game& game, int depth, bool isMaximizing, int alpha, int beta, int id, int currentScore) {
    if (game.getEnd())
        return evaluateBoard(game, id);
    if (depth == 0)
        return currentScore;

    std::vector<Move> moves = getAvailableMoves(game);
    if (moves.empty())
        return currentScore;

    int ennemyId = (id == 1) ? 2 : 1;
    int currentId = isMaximizing ? id : ennemyId;

    std::vector<std::pair<int, Move>> scored;
    scored.reserve(moves.size());
    for (const Move& m : moves)
        scored.push_back({_scoreMove(game, m.x, m.y, currentId), m});
    std::sort(scored.begin(), scored.end(), [](const std::pair<int, Move>& a, const std::pair<int, Move>& b) {
        return a.first > b.first;
    });
    const int MAX_CAND = 7;
    int n = std::min((int)scored.size(), MAX_CAND);

    if (isMaximizing) {
        int maxEval = -2000000;
        for (int i = 0; i < n; i++) {
            const Move& move = scored[i].second;
            int c1 = game.getPlayer1().getCaptures();
            int c2 = game.getPlayer2().getCaptures();
            int capSize = (int)game.getCaptured().size();
            bool oldEnd = game.getEnd();
            int oldWinner = game.getWinnerId();

            int beforeLines = _computeLineScore(game, move.x, move.y, id, ennemyId);
            game.getBoard().setCell(move.x, move.y, id);
            game.updateState(move.x, move.y);
            int afterLines = _computeLineScore(game, move.x, move.y, id, ennemyId);
            int captDelta = (id == 1)
                ? ((game.getPlayer1().getCaptures() - c1) - (game.getPlayer2().getCaptures() - c2)) * 300
                : ((game.getPlayer2().getCaptures() - c2) - (game.getPlayer1().getCaptures() - c1)) * 300;
            int delta = (afterLines - beforeLines) + captDelta;

            int eval = minimax(game, depth - 1, false, alpha, beta, id, currentScore + delta);

            game.getBoard().setCell(move.x, move.y, 0);
            int captured = (int)game.getCaptured().size() - capSize;
            if (captured > 0) game.resetCaptures(captured);
            game.getPlayer1().setCaptures(c1);
            game.getPlayer2().setCaptures(c2);
            game.setEnd(oldEnd);
            game.setWinnerId(oldWinner);

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) break;
        }
        return maxEval;
    } else {
        int minEval = 2000000;
        for (int i = 0; i < n; i++) {
            const Move& move = scored[i].second;
            int c1 = game.getPlayer1().getCaptures();
            int c2 = game.getPlayer2().getCaptures();
            int capSize = (int)game.getCaptured().size();
            bool oldEnd = game.getEnd();
            int oldWinner = game.getWinnerId();

            int beforeLines = _computeLineScore(game, move.x, move.y, id, ennemyId);
            game.getBoard().setCell(move.x, move.y, ennemyId);
            game.updateState(move.x, move.y);
            int afterLines = _computeLineScore(game, move.x, move.y, id, ennemyId);
            int captDelta = (id == 1)
                ? ((game.getPlayer1().getCaptures() - c1) - (game.getPlayer2().getCaptures() - c2)) * 300
                : ((game.getPlayer2().getCaptures() - c2) - (game.getPlayer1().getCaptures() - c1)) * 300;
            int delta = (afterLines - beforeLines) + captDelta;

            int eval = minimax(game, depth - 1, true, alpha, beta, id, currentScore + delta);

            game.getBoard().setCell(move.x, move.y, 0);
            int captured = (int)game.getCaptured().size() - capSize;
            if (captured > 0) game.resetCaptures(captured);
            game.getPlayer1().setCaptures(c1);
            game.getPlayer2().setCaptures(c2);
            game.setEnd(oldEnd);
            game.setWinnerId(oldWinner);

            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

// --- play() : version qui retourne le meilleur coup ---
std::pair<int, int> Agent::play() {
    Game game = getGameCopy();
    std::vector<Move> moves = getAvailableMoves(game);
    if (moves.empty()) return {-1, -1};

    int id = game.getCurrentPlayer().getId();
    int ennemyId = (id == 1) ? 2 : 1;
    int bestScore = -2000000;
    Move bestMove = moves[0];
    const int depth = 8;
    const int MAX_CAND = 7;

    std::vector<std::pair<int, Move>> scored;
    scored.reserve(moves.size());
    for (const Move& m : moves)
        scored.push_back({_scoreMove(game, m.x, m.y, id), m});
    std::sort(scored.begin(), scored.end(), [](const std::pair<int, Move>& a, const std::pair<int, Move>& b) {
        return a.first > b.first;
    });
    int n = std::min((int)scored.size(), MAX_CAND);

    for (int i = 0; i < n; i++) {
        const Move& move = scored[i].second;
        int c1 = game.getPlayer1().getCaptures();
        int c2 = game.getPlayer2().getCaptures();
        int capSize = (int)game.getCaptured().size();
        bool oldEnd = game.getEnd();
        int oldWinner = game.getWinnerId();

        int beforeLines = _computeLineScore(game, move.x, move.y, id, ennemyId);
        game.getBoard().setCell(move.x, move.y, id);
        game.updateState(move.x, move.y);
        int afterLines = _computeLineScore(game, move.x, move.y, id, ennemyId);
        int captDelta = (id == 1)
            ? ((game.getPlayer1().getCaptures() - c1) - (game.getPlayer2().getCaptures() - c2)) * 300
            : ((game.getPlayer2().getCaptures() - c2) - (game.getPlayer1().getCaptures() - c1)) * 300;
        int delta = (afterLines - beforeLines) + captDelta;

        int score = minimax(game, depth - 1, false, -2000000, 2000000, id, delta);

        game.getBoard().setCell(move.x, move.y, 0);
        int captured = (int)game.getCaptured().size() - capSize;
        if (captured > 0) game.resetCaptures(captured);
        game.getPlayer1().setCaptures(c1);
        game.getPlayer2().setCaptures(c2);
        game.setEnd(oldEnd);
        game.setWinnerId(oldWinner);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    printf("x : %d, y : %d | score: %d\n", bestMove.x, bestMove.y, bestScore);
    return {bestMove.x, bestMove.y};
}


int Agent::_scoreLine(const int* line, int n, int playerId) {
    int score = 0;
    for (int i = 0; i < n; i++) {
        if (line[i] != playerId) continue;
        if (i > 0 && line[i - 1] == playerId) continue;

        int size = 1, closed = 0;
        bool hole = false;

        if (i == 0 || line[i - 1] != 0) closed++;

        int j = i + 1;
        while (j < n) {
            int cell = line[j];
            if (cell != playerId) {
                if (cell == 0) {
                    if (j + 1 >= n) break;
                    int nextCell = line[j + 1];
                    if (nextCell == 0) break;
                    else if (nextCell == playerId) {
                        if (hole) break;
                        hole = true;
                        j++;
                        continue;
                    }
                    else break;
                } else {
                    closed++;
                    break;
                }
            } else {
                size++;
            }
            j++;
        }
        if (j >= n) closed++;
        score += _getAllignementValue(size, closed, hole);
    }
    return score;
}

int Agent::_computeLineScore(Game& game, int x, int y, int playerId, int enemyId) {
    std::vector<std::pair<int, int>> dirs = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    int score = 0;

    for (auto [dx, dy] : dirs) {
        int line[19];
        int n = 0;

        int sx = x, sy = y;
        while (_isInLimit(sx - dx, sy - dy)) { sx -= dx; sy -= dy; }

        int cx = sx, cy = sy;
        while (_isInLimit(cx, cy)) {
            line[n++] = game.getBoard().getCell(cx, cy);
            cx += dx; cy += dy;
        }

        score += _scoreLine(line, n, playerId) - _scoreLine(line, n, enemyId);
    }
    return score;
}

int Agent::_scoreMove(Game& game, int x, int y, int id) {
    int ennemyId = (id == 1) ? 2 : 1;
    int score = 0;
    std::vector<std::pair<int, int>> dirs = {{1,0},{0,1},{1,1},{1,-1}};
    int sizePoints[6] = {0, 1, 10, 100, 10000, 100000};

    for (auto [dx, dy] : dirs) {
        for (int playerId : {id, ennemyId}) {
            int count = 0;
            for (int sign : {-1, 1}) {
                for (int i = 1; i <= 4; i++) {
                    int nx = x + sign * i * dx;
                    int ny = y + sign * i * dy;
                    if (!_isInLimit(nx, ny) || game.getBoard().getCell(nx, ny) != playerId)
                        break;
                    count++;
                }
            }
            int pts = sizePoints[std::min(count, 5)];
            score += (playerId == id) ? pts * 2 : pts;
        }
    }
    return score;
}

Game& Agent::getGame(void) const {
    return *_game;
}

Game Agent::getGameCopy(void) const {
    return *_game;
}
