#ifndef PLAYER_HPP
# define PLAYER_HPP
# include <string>

class Player {

	public:
		Player(void);
		Player(std::string name);
		Player(std::string name, int score);
		Player(const Player& other);
		~Player();

		const std::string getName() const;
		int getScore() const;

	private:
		std::string _name;
		int _score;

};

#endif