#include "Display.hpp"

Display::Display(void) : _board(Board()), _game(Game(_board)) {}

Display::Display(Board board) : _board(board), _game(Game(_board)) {}

Display::Display(const Display& other) : _board(other.getBoard()), _game(other.getGame()) {}

Display::~Display() {}

const Board Display::getBoard(void) const {
	return _board;
}

const Game Display::getGame(void) const {
	return _game;
}

void Display::open(void) {
	int windowSize = _cellSize * _gridSize;
	sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Gomoku", sf::Style::Close);
	while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
			if (event.type == sf::Event::MouseButtonPressed) {
                int x = event.mouseButton.x / _cellSize;
                int y = event.mouseButton.y / _cellSize;
                 if (event.mouseButton.button == sf::Mouse::Left) {
					if (_game.moveIsValid(x, y, 1)) {
						_board.setCell(x, y, 1);
						_game.nextTurn();
					}
                } 
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    if (_game.moveIsValid(x, y, 2)) {
						_board.setCell(x, y, 2);
						_game.nextTurn();
					}
                }
            }
        }
		window.clear(sf::Color(240, 217, 181));
        _drawGrid(window);
        _drawPieces(window);
        window.display();
    }
}

void Display::_drawGrid(sf::RenderWindow& window) {
	sf::RectangleShape line;
	float ray = _cellSize / 2.5f;

	for (int x = 0; x <= _gridSize; x++) {
		line.setSize(sf::Vector2f(2, _cellSize * _gridSize));
		line.setFillColor(sf::Color::Black);
		line.setPosition(x * _cellSize + ray, 0);
		window.draw(line);
	}

	for (int y = 0; y <= _gridSize; y++) {
		line.setSize(sf::Vector2f(_cellSize * _gridSize, 2));
		line.setFillColor(sf::Color::Black);
		line.setPosition(0, y * _cellSize + ray);
		window.draw(line);
	}
}

void Display::_drawPieces(sf::RenderWindow& window) {
	for (int y = 0; y < _gridSize; y++) {
		for (int x = 0; x < _gridSize; x++) {
			int cell = _board.getCell(x, y); // 0 = vide, 1 = noir, 2 = blanc
			if (cell == 0)
				continue;

			sf::CircleShape piece(_cellSize / 2.5f);
			piece.setPosition(x * _cellSize, y * _cellSize);
			if (cell == 1)
				piece.setFillColor(sf::Color::Black);
			else if (cell == 2)
				piece.setFillColor(sf::Color::White);
			window.draw(piece);
		}
	}
}
