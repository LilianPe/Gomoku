#ifndef PLAYER_HPP
# define PLAYER_HPP
# include <string>

class Player {

	public:
		Player(void);
		Player(std::string name);
		Player(std::string name, int score, int captures);
		Player(const Player& other);
		~Player();

		const std::string getName() const;
		int getScore() const;
		int getCaptures() const;
		void incrementCaptures(int n);
		void resetCaptures();

	private:
		std::string _name;
		int _score;
		int _captures;

};

#endif