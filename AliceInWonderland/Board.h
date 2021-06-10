#ifndef ALICEBOARD
#define ALICEBOARD
#include <vector> 
#include <SFML/Graphics.hpp>
class Board {
private:
	sf::RenderWindow window{ sf::VideoMode(800, 600), "SFML window" };
	std::vector<int> tiles;
	size_t width;
	size_t height;
public:

};
#endif
