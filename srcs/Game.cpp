#include "Game.hpp"
# include "Board.hpp"
# include "Agent.hpp"

Game::Game(std::shared_ptr<Board> board) :
	_player1(Player("Player1", "Player")),
	_player2(Player("Player2", "Player")),
	_board(board),
	_currentTurn(1),
	_winner(0),
	_end(false),
	_endReason(""),
	_agent(Agent(*this)),
	_gameType(PVSP) {}

Game::Game(std::shared_ptr<Board> board, Player player1, Player player2) : 
	_player1(player1),
	_player2(player2),
	_board(board),
	_currentTurn(1),
	_winner(0),
	_end(false),
	_endReason(""),
	_agent(Agent(*this)),
	_gameType(PVSP) {}

Game::Game(const Game& other) :
	_player1(Player(other._player1)),
	_player2(Player(other._player2)),
	_board(std::make_shared<Board>(*other._board)),
	_currentTurn(other.getCurrentTurn()),
	_winner(other.getWinnerId()),
	_end(other.getEnd()),
	_endReason(other.getEndReason()),
	_agent(Agent(other.getAgent())) {}

Game::~Game() {}

void Game::restart(void) {
	getBoard().clear();
	_currentTurn = 1;
	_winner = 0;
	_end = false;
	_player1.resetCaptures();
	_player2.resetCaptures();
}

void Game::displayBoard(void) {
	getBoard().display();
}

// void Game::launch() {
// 	if (getCurrentPlayer().getType() == "AI") {
// 		auto [x, y] = _agent.play();
// 		getBoard().setCell(x, y, getCurrentTurn());
// 		updateState(x, y);
// 		if (getEnd()) {
// 			try {
// 				std::cout << "Winner :" << getWinner().getName() << std::endl; 
// 			}
// 			catch (const std::logic_error& e) {
// 				std::cout << "Error: " << e.what() << std::endl;
// 			}
// 		}
// 		nextTurn();
// 	}
// }

void Game::nextTurn(void) {
	if (getCurrentPlayer().getType() == "AI") {
		auto [x, y] = _agent.play();
		printf("x: %d | y: %d | color: %d\n", x, y, getCurrentTurn());
        getBoard().setCell(x, y, getCurrentTurn());
		updateState(x, y);
		if (getEnd()) {
			try {
				std::cout << "Winner :" << getWinner().getName() << std::endl; 
			}
			catch (const std::logic_error& e) {
				std::cout << "Error: " << e.what() << std::endl;
			}
			return ;
		}
	}
	if (_currentTurn == 1) {
		_currentTurn = 2;
	} else {
		_currentTurn = 1;
	}
}

bool Game::moveIsValid(int x, int y, int p) {
	if (p != _currentTurn) {
		return false;
	}
	if (getBoard().getCell(x, y) != 0) {
		return false;
	}
	return !_end;
}


std::vector<Move> Game::getPlayerPawn(int x, int y) {
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
	int colour = getBoard().getCell(x, y);
	std::vector<Move> pawns;
	pawns.push_back({x, y});
	for (auto [dx, dy] : directions) {
		for (int i = 1; i < 3; i++) {
			int x2 = x + i * dx;
			int y2 = y + i * dy;
			if (!_isInLimit(x2, y2)) continue;
			if (getBoard().getCell(x2, y2) == colour) {
				pawns.push_back({x2, y2});
			}
		}
	}
	return pawns;
}

void Game::updateState(int x, int y) {
	_checkFive(x, y);
	std::vector<Move> pawns = getPlayerPawn(x, y);
	for (Move& m : pawns) {
		_checkDoubleThree(m.x, m.y);
	}
	_checkCapture(x, y);
}

void Game::_checkFive(int x, int y) {
	int cell = 0;
	int n = 0;
	int count = 0;
	std::vector<std::pair<int, int>> aligned_points;

	std::vector<std::pair<int, int>> directions = {
		{1, 0},
		{1, 1},
		{0, 1},
		{-1, 1}
	};
	for (auto [dx, dy] : directions) {
		cell = getBoard().getCell(x, y);
		n = 1;
		count = 1;
		aligned_points.push_back({x, y});
		while (n < 5 &&
			_isInLimit(x + n * dx, y + n * dy) &&
			getBoard().getCell(x + n * dx, y + n * dy) == cell) {
			aligned_points.push_back({x + n * dx, y + n * dy});
			n++;
			count++;
		}
		n = -1;
		while (n > -5 && 
			_isInLimit(x + n * dx, y + n * dy) &&
			getBoard().getCell(x + n * dx, y + n * dy) == cell) {
			aligned_points.push_back({x + n * dx, y + n * dy});
			n--;
			count++;
		}
		if (count >= 5 && !_areCapturables(aligned_points)) {
			// printf("ENDING");
			_end = true;
			_winner = cell;
			_endReason = "5 pawns aligned.";
			return;
		}
		aligned_points.clear();
	}
}

void Game::_checkDoubleThree(int x, int y) {
	int pawnCell = 0;
	int cell = 0;
	int n = 0;
	int count = 0;
	int threes = 0;
	int emptyFirst = 0;
	int emptySecond = 0;
	int emptyMid = 0;
	std::vector<std::pair<int, int>> directions = {
		{1, 0},
		{1, 1},
		{0, 1},
		{-1, 1}
	};
	for (auto [dx, dy] : directions) {
		pawnCell = getBoard().getCell(x, y);
		n = 1;
		count = 1;
		emptyFirst = 0;
		emptySecond = 0;
		emptyMid = 0;
		while (n < 5 && _isInLimit(x + n * dx, y + n * dy)) {
			cell = getBoard().getCell(x + n * dx, y + n * dy);
			if (cell == pawnCell) {
				count++;
			}
			else if (cell == 0) {
				if (!emptyMid) {
					emptyMid = 1;
				}
				else if (emptyMid == 1 && getBoard().getCell(x + (n - 1) * dx, y + (n - 1) * dy) == 0) {
					emptyMid = 0;
					emptyFirst = 1;
					break;
				}
				else if (emptyMid == 1 && getBoard().getCell(x + (n - 1) * dx, y + (n - 1) * dy) == pawnCell) {
					emptyFirst = 1;
					break;
				}
			}
			else {
				break;
			}
			n++;
		}
		n = -1;
		while (abs(n) < 5 && _isInLimit(x + n * dx, y + n * dy)) {
			cell = getBoard().getCell(x + n * dx, y + n * dy);
			if (cell == pawnCell) {
				count++;
			}
			else if (cell == 0) {
				if (!emptyMid) {
					emptyMid = 1;
				}
				else if (emptyMid == 1 && getBoard().getCell(x + (n + 1) * dx, y + (n + 1) * dy) == 0) {
					emptyMid = 0;
					emptySecond = 1;
					break;
				}
				else if (emptyMid == 1 && getBoard().getCell(x + (n + 1) * dx, y + (n + 1) * dy) == pawnCell) {
					emptySecond = 1;
					break;
				}
			}
			else {
				break;
			}
			n--;
		}
		if (count == 3 && emptyFirst == 1 && emptySecond == 1) {
			threes++;
		}
	}
	if (threes >= 2) {
		_end = true;
		_endReason = "DoubleThree are forbidden.";
		_winner = (pawnCell == 1) ? 2 : 1;
		return ;
	}
}

void Game::_checkCapture(int x, int y) {
	int cell = getBoard().getCell(x, y);
	int opponentCell = 0;
	if (cell == 1) {
		opponentCell = 2;
	} else {
		opponentCell = 1;
	}
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
		int x2 = x + 2 * dx;
		int y2 = y + 2 * dy;
		if (_isInLimit(x2, y2) &&
			getBoard().getCell(x1, y1) == opponentCell &&
			getBoard().getCell(x2, y2) == opponentCell) {
			int x3 = x + 3 * dx;
			int y3 = y + 3 * dy;
			if (_isInLimit(x3, y3) &&
				getBoard().getCell(x3, y3) == cell) {
				getBoard().setCell(x1, y1, 0);
				getBoard().setCell(x2, y2, 0);
				Move m1 = {x1, y1};
				Move m2 = {x2, y2};
				_captured.push_back({m1, m2, opponentCell});
				if (cell == 1) {
					_player1.incrementCaptures(2);
				} else {
				_player2.incrementCaptures(2);
				}
			}
		}
	} 
	// Cheking for 10 captures
	if (_player1.getCaptures() == 8) {

		std::vector<std::pair<int, int>> points;
		for (int x = 0; x < SIZE; x++){
			for (int y = 0; y < SIZE; y++) {
				if (getBoard().getCell(x, y) == 2)
					points.push_back({x, y});
			}
		}
		if (_areCapturables(points)) {
			_end = true;
			_endReason = "10 pawn captured.";
			_winner = 1;
			return ;
		}
	}
	if (_player2.getCaptures() == 8) {

		std::vector<std::pair<int, int>> points;
		for (int x = 0; x < SIZE; x++){
			for (int y = 0; y < SIZE; y++) {
				if (getBoard().getCell(x, y) == 1)
					points.push_back({x, y});
			}
		}
		if (_areCapturables(points)) {
			_end = true;
			_endReason = "10 pawn captured.";
			_winner = 2;
			return ;
		}
	}
}

bool Game::_areCapturables(const std::vector<std::pair<int, int>>& points) {
	for (const auto& [x, y] : points) {
		int cell = getBoard().getCell(x, y);
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
			if (getBoard().getCell(x1, y1) != cell) continue; // has ally cell around

	
			if (!_isInLimit(nxBefore, nyBefore) || !_isInLimit(nxAfter, nyAfter)) continue;
			if (getBoard().getCell(nxBefore, nyBefore) == cell || getBoard().getCell(nxAfter, nyAfter) == cell) continue;
			if (getBoard().getCell(nxBefore, nyBefore) == 0) {
				if (getBoard().getCell(nxAfter, nyAfter) == 0) {
					continue;
				}
			}
			return true; // Point is capturable
		}
	}
	return false;
}

void Game::resetCaptures(int n) {
	while (!_captured.empty() && n) {
		n--;
		Capture capture = _captured.back();
		_captured.pop_back();
		getBoard().setCell(capture.m1.x, capture.m1.y, capture.index);
		getBoard().setCell(capture.m2.x, capture.m2.y, capture.index);
	}
}

bool Game::_isInLimit(int x, int y) {
	return (x >= 0 && x < SIZE && y >= 0 && y < SIZE);
}

int Game::getCurrentTurn(void) const {
	return _currentTurn;
}

Player& Game::getPlayer1(void) {
	return _player1;
}

Player& Game::getPlayer2(void) {
	return _player2;
}

void Game::setBoard(Board& board) {
	*_board = board;
}

Board& Game::getBoard(void) {
	return *_board;
}

const Board& Game::getBoard(void) const {
	return *_board;
}

bool Game::getEnd(void) const {
	return _end;
}

void Game::setEnd(bool end) {
	_end = end;
}

std::string Game::getEndReason(void) const {
	return _endReason;
}

int Game::getWinnerId(void) const {
	return _winner;
}

void Game::setWinnerId(int id) {
	_winner = id;
}

Player Game::getWinner(void) const {
	if (_winner == 1) return _player1;
	else if (_winner == 2) return _player2;
	else throw std::logic_error("No winner!");
}

Agent Game::getAgent(void) const {
	return _agent;
}

Player Game::getCurrentPlayer(void) const {
	if (_currentTurn == 1) return _player1;
	else return _player2;
}

std::vector<Capture>& Game::getCaptured(void) {
	return _captured;
}

GameType Game::getGameType(void) {
	return _gameType;
}

void Game::setGameType(GameType type) {
	_gameType = type;
}

// Ajouter un bitBoard et modifier les verifications  pour chercher des patterns via bitShifting
// Base pour set le BitBoard
//
// En gros, set toutes les lignes / colones / diagonales a 0
// Puis a chaque coup, placer comme cela le pion:
// Fait un OR entre la ligne et une ligne 0 avec 1 a la position x ou y
// black_rows[x] |= (1u << y);
// black_cols[y] |= (1u << x);


// Revoir comment set les diagonales
// int diag_index = x + y;           // de 0 à 36
// int diag2_index = x - y;           // de 0 à 36
// int bit_pos    = std::min(x, y);  // de 0 à 18 → parfait pour uint32_t

// black_diag1[diag_index] |= (1u << bit_pos);





// class BitBoard {
//     uint32_t b_rows[19] = {0};
//     uint32_t w_rows[19] = {0};
//     uint32_t b_cols[19] = {0};
//     uint32_t w_cols[19] = {0};
//     uint32_t b_diag1[37] = {0};  // x+y
//     uint32_t w_diag1[37] = {0};
//     uint32_t b_diag2[37] = {0};  // x-y+18
//     uint32_t w_diag2[37] = {0};

// public:
//     void build_from_board(int board[19][19]) {
//         memset(b_rows, 0, sizeof(b_rows));
//         memset(w_rows, 0, sizeof(w_rows));
//         // etc. pour tous

//         for (int x = 0; x < 19; x++) {
//             for (int y = 0; y < 19; y++) {
//                 uint32_t mask = (1u << y);  // bit y dans la ligne x

//                 if (board[x][y] == 1) {  // noir
//                     b_rows[x] |= mask;
//                     b_cols[y] |= (1u << x);
//                     b_diag1[x + y] |= (1u << std::min(x, y));
//                     b_diag2[x - y + 18] |= (1u << std::min(x, 18 - y));
//                 }
//                 else if (board[x][y] == 2) {  // blanc
//                     w_rows[x] |= mask;
//                     w_cols[y] |= (1u << x);
//                     w_diag1[x + y] |= (1u << std::min(x, y));
//                     w_diag2[x - y + 18] |= (1u << std::min(x, 18 - y));
//                 }
//             }
//         }
//     }

//     void place(int x, int y, int player) {
//         uint32_t mask_row = (1u << y);
//         uint32_t mask_col = (1u << x);
//         uint32_t mask_d1 = (1u << std::min(x, y));
//         uint32_t mask_d2 = (1u << std::min(x, 18 - y));

//         if (player == 1) {
//             b_rows[x] |= mask_row;
//             b_cols[y] |= mask_col;
//             b_diag1[x + y] |= mask_d1;
//             b_diag2[x - y + 18] |= mask_d2;
//         } else {
//             w_rows[x] |= mask_row;
//             w_cols[y] |= mask_col;
//             w_diag1[x + y] |= mask_d1;
//             w_diag2[x - y + 18] |= mask_d2;
//         }
//     }

//     void remove(int x, int y, int player) {
//         uint32_t mask_row = ~(1u << y);
//         uint32_t mask_col = ~(1u << x);
//         uint32_t mask_d1 = ~(1u << std::min(x, y));
//         uint32_t mask_d2 = ~(1u << std::min(x, 18 - y));

//         if (player == 1) {
//             b_rows[x] &= mask_row;
//             b_cols[y] &= mask_col;
//             b_diag1[x + y] &= mask_d1;
//             b_diag2[x - y + 18] &= mask_d2;
//         } else {
//             // idem pour blanc
//         }
//     }
// };