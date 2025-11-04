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
	sf::Font font;
    font.loadFromFile("assets/arial.ttf");
	while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

			if (_game.getEnd()) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;
                    // Button "Play again"
                    if (mouseX > windowSize/2 - 100 && mouseX < windowSize/2 + 100 &&
                        mouseY > windowSize/2 && mouseY < windowSize/2 + 50) {
                            _game.restart();
                    }
                    // Button "Leave"
                    else if (mouseX > windowSize/2 - 100 && mouseX < windowSize/2 + 100 &&
                             mouseY > windowSize/2 + 70 && mouseY < windowSize/2 + 120) {
                        window.close();
                    }
                }
                continue; // empêche de jouer pendant l’écran de fin
            }

			if (event.type == sf::Event::MouseButtonPressed) {
                int x = event.mouseButton.x / _cellSize;
                int y = event.mouseButton.y / _cellSize;
                if (event.mouseButton.button == sf::Mouse::Left) {
					if (_game.moveIsValid(x, y, 1)) {
						_board.setCell(x, y, 1);
						_game.updateState(x, y);
						if (_game.getEnd()) {
                            try {
                                std::cout << "Winner :" << _game.getWinner().getName() << std::endl; 
                            }
                            catch (const std::logic_error& e) {
                                std::cout << "Error: " << e.what() << std::endl;
                            }
						}
						_game.nextTurn();
					}
                } 
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    if (_game.moveIsValid(x, y, 2)) {
						_board.setCell(x, y, 2);
						_game.updateState(x, y);
						if (_game.getEnd()) {
                            try {
                                std::cout << "Winner :" << _game.getWinner().getName() << std::endl; 
                            }
                            catch (const std::logic_error& e) {
                                std::cout << "Error: " << e.what() << std::endl;
                            }
						}
						_game.nextTurn();
					}
                }
            }
        }
		window.clear(sf::Color(240, 217, 181));
        _drawGrid(window);
        _drawPieces(window);
        _drawScores(window, windowSize, font);
		if (_game.getEnd()) {
            sf::RectangleShape overlay(sf::Vector2f(windowSize, windowSize));
            overlay.setFillColor(sf::Color(0, 0, 0, 150)); // fond semi-transparent
            window.draw(overlay);
			std::string message;
            try {
                message = "Winner: " + _game.getWinner().getName();
            }
            catch (const std::logic_error& e) {
                message = "Error";
            }
            sf::Text text(message, font, 50);
            text.setFillColor(sf::Color::White);
            text.setPosition(windowSize / 2 - 150, windowSize / 2 - 100);
            window.draw(text);

            // Bouton Rejouer
            sf::RectangleShape replayButton(sf::Vector2f(200, 50));
            replayButton.setPosition(windowSize / 2 - 100, windowSize / 2);
            replayButton.setFillColor(sf::Color(100, 200, 100));
            window.draw(replayButton);

            sf::Text replayText("Play again", font, 24);
            replayText.setFillColor(sf::Color::Black);
            replayText.setPosition(windowSize / 2 - 55, windowSize / 2 + 10);
            window.draw(replayText);

            // Bouton Quitter
            sf::RectangleShape quitButton(sf::Vector2f(200, 50));
            quitButton.setPosition(windowSize / 2 - 100, windowSize / 2 + 70);
            quitButton.setFillColor(sf::Color(200, 100, 100));
            window.draw(quitButton);

            sf::Text quitText("Quitter", font, 24);
            quitText.setFillColor(sf::Color::Black);
            quitText.setPosition(windowSize / 2 - 45, windowSize / 2 + 80);
            window.draw(quitText);
        }
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

void Display::_drawScores(sf::RenderWindow& window, int windowSize, sf::Font& font) {
        std::string score_P1;
        std::string score_P2;

        score_P1 = "Player1 : " + std::to_string(_game.getPlayer1().getCaptures());
        score_P2 = "Player2 : " + std::to_string(_game.getPlayer2().getCaptures());

        sf::Text text1(score_P1, font, 20);
        text1.setFillColor(sf::Color::Black);
        text1.setPosition(windowSize / 98, (windowSize / 100) * 96);
        window.draw(text1);
        
        sf::Text text2(score_P2, font, 20);
        text2.setFillColor(sf::Color::Black);
        text2.setPosition(windowSize / 98, (windowSize / 100) * 99);
        window.draw(text2);
}