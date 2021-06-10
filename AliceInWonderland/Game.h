#ifndef ALICE_GAME
#define ALICE_GAME
#include "Characters.h"
#include <vector>
#include <fstream>
enum class Direction {Up, Down, Left, Right};
class Game{
private:
	Player player;
	std::vector<Enemy> enemies;
	unsigned tiles_width=0;
	unsigned tiles_height=0;
	unsigned levels = 2;
	int selected_item = -1;
	unsigned lastEnemyHp = 0;
	sf::Font displayFont;
	sf::Text displayPlayerStats;
	sf::Event changer;
	sf::RenderWindow* window;
	sf::RectangleShape generate;
	sf::View view{ sf::View(sf::Vector2f(250.0f, 275.0f), sf::Vector2f(500.0f, 550.0f)) };
	std::vector<sf::Texture> tiles_t;//contain the textures for potions, portals, wall and floor
	//tile id 0 = path, 1 = wall, 2 = enter gate, 3 = exit gate, 4 = PotionOfTeleport, 5 = DrinkMe, 6 = Eatme 7 = MagicFan
	//8 = InvisiHat, 9 = Rose
	std::vector<unsigned> tile_ids;
public:
	Game() {
		this->window = new sf::RenderWindow(sf::VideoMode(512, 512), "AliceInWonderland", sf::Style::Close | sf::Style::Resize);
		this->generate.setSize(sf::Vector2f(50.0f, 50.0f));
		displayFont.loadFromFile("../Fonts/ArialCE.ttf");
		displayPlayerStats.setFont(displayFont);
		displayPlayerStats.setCharacterSize(15);
		loadFromFile("../Levels/level1.lvl");
		initTextures();
		
	}
	~Game() {
		delete window;
	}
	void initTextures() {
		//Initiates all of the board textures.
		sf::Texture temp;
		temp.loadFromFile("../Textures/path.png");
		tiles_t.push_back(temp);
		temp.loadFromFile("../Textures/bush.png");
		tiles_t.push_back(temp);
		temp.loadFromFile("../Textures/enter.png");
		tiles_t.push_back(temp);
		temp.loadFromFile("../Textures/exit.png");
		tiles_t.push_back(temp);
		temp.loadFromFile("../Textures/teleportation.png");
		tiles_t.push_back(temp);
		temp.loadFromFile("../Textures/drinkme.png");
		tiles_t.push_back(temp);
		temp.loadFromFile("../Textures/eatme.png");
		tiles_t.push_back(temp);
		temp.loadFromFile("../Textures/fan.png");
		tiles_t.push_back(temp);
		temp.loadFromFile("../Textures/invisihat.png");
		tiles_t.push_back(temp);
		temp.loadFromFile("../Textures/rose.png");
		tiles_t.push_back(temp);
	}
	void start() {
		this->window->setKeyRepeatEnabled(false);
		while (this->window->isOpen()) {
			while (this->window->pollEvent(changer)) {
				switch (this->changer.type) {
				case sf::Event::Closed:
					this->window->close();
					break;
				case sf::Event::Resized:
					resizeView();
					break;
				}
			}
			this->window->clear(sf::Color(170, 138, 100));
			this->window->setView(view);
			handleEventTiles();
			if (this->changer.type == sf::Event::KeyReleased) {
				handleMovement();
				handleItemSelect();
			}
			renderTiles(this->window);
			renderEnemies();
			this->player.render(window);
			renderPlayerStats(window);
			this->window->display();
		}
	}
	void resizeView();
	void loadFromFile(std::string filename);
	void levelLoading();
	void handleItemSelect();
	void handleMovement();
	bool willFightEnemy(unsigned x, unsigned y);
	void handleCombat(Enemy* foe, Direction direction);//needs direction for teleport
	bool checkEnemyThere(unsigned x, unsigned y);
	Enemy* getEnemyThere(unsigned x, unsigned y);
	bool canTeleportThere(unsigned x, unsigned y);
	ItemType onItemUse(unsigned& damage, Direction direction);//needs damage value so that it can modify it
	void potionOfTeleport(Direction direction);
	void onPhaseExit();
	void handleEventTiles();
	void renderTiles(sf::RenderWindow* window);
	void renderEnemies();
	void renderPlayerStats(sf::RenderWindow* window);
	void onPlayerDeath();
};
#endif
