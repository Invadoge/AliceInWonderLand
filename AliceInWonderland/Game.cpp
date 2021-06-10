#include "Game.h"

void Game::resizeView() {
	float aspectratio = float(window->getSize().x) / float((window->getSize().y));
	view.setSize(512.0f * aspectratio, 512.0f);
}
void Game::loadFromFile(std::string filename) {
	std::ifstream level(filename, std::ios::in | std::ios::binary);
	selected_item = -1;
	this->enemies.clear();
	this->tile_ids.clear();
	if (level.is_open()) {
		unsigned pos_x = 0, pos_y = 0;
		//First we set the player position
		level >> pos_x >> pos_y;
		this->player.set_position(pos_x, pos_y);
		this->player.clearInventory();
		this->player.set_hp(100);
		unsigned enemy_count = 0;
		unsigned e_type = 0;
		//we add the number of enemies
		level >> enemy_count;
		for (unsigned i = 0; i < enemy_count; i++) {
			//after that we add their type and coordinates
			level >> e_type >> pos_x >> pos_y;
			enemies.push_back(Enemy{ (EnemyType)e_type, pos_x, pos_y });
		}
		//we get the level hight and width
		level >> tiles_width >> tiles_height;
		unsigned tile_id = 0;
		for (unsigned i = 0; i < tiles_height * tiles_width; i++) {
			//finally we set each individual tile
			level >> tile_id;
			tile_ids.push_back(tile_id);
		}
	}
}
void Game::levelLoading(){
	//here we could implement a level loader for more levels
	//for now we have only two levels
	//since we load the first level from initialisation we start by checking for second level
	//if we were to implement the system fully, levels will be a higher number and we will start counting down
	if (levels == 2) {
		levels--;
		loadFromFile("../Levels/level2.lvl");
	}else if (levels == 1) {
		window->close();
	}
}
void Game::handleItemSelect(){
	if (this->changer.key.code == sf::Keyboard::Key::Num1) {
		selected_item = 0;
		this->changer.key.code = sf::Keyboard::Key::Unknown;//implementing it here, because of handleMovement()
	}
	if (this->changer.key.code == sf::Keyboard::Key::Num2) {
		selected_item = 1;
		this->changer.key.code = sf::Keyboard::Key::Unknown;//implementing it here, because of handleMovement()
	}
	if (this->changer.key.code == sf::Keyboard::Key::Num3) {
		selected_item = 2;
		this->changer.key.code = sf::Keyboard::Key::Unknown;//implementing it here, because of handleMovement()
	}
}
bool Game::checkEnemyThere(unsigned x, unsigned y) {
	unsigned size = enemies.size();
	for (unsigned i = 0; i < size; i++) {
		if (enemies[i].get_x() == x && enemies[i].get_y() == y && enemies[i].getAlive())return true;
	}
	return false;
}

Enemy* Game::getEnemyThere(unsigned x, unsigned y){
	unsigned size = enemies.size();
	for (unsigned i = 0; i < size; i++) {
		if (enemies[i].get_x() == x && enemies[i].get_y() == y && enemies[i].getAlive())return &enemies[i];
	}
}

bool Game::canTeleportThere(unsigned x, unsigned y){
	//check if an enemy is present
	if (checkEnemyThere(x, y))return false;
	//check if the tile is a wall
	unsigned tile_coords = x + y * tiles_height;
	if (tile_ids[tile_coords] == 1)return false;
	return true;
}

void Game::handleMovement() {
	if (changer.key.code == sf::Keyboard::Key::Up) {
		if (player.get_y() != 0 &&
			tile_ids[player.get_x() + (player.get_y() - 1) * tiles_width] != 1) {
			if (willFightEnemy(player.get_x(), player.get_y() - 1)) {
				handleCombat(getEnemyThere(player.get_x(), player.get_y() - 1), Direction::Up);
			}
			else {
				player.movePlayer(0, -1);
			}
			if (player.decreasePhase() && !player.canPhase())onPhaseExit();
		}
		changer.key.code = sf::Keyboard::Key::Unknown;//implementing it here, because of handleItemSelect()
	}
	if (changer.key.code == sf::Keyboard::Key::Down) {
		if (player.get_y() < tiles_height &&
			tile_ids[player.get_x() + (player.get_y() + 1) * tiles_width] != 1) {
			if (willFightEnemy(player.get_x(), player.get_y() + 1)) {
				handleCombat(getEnemyThere(player.get_x(), player.get_y() + 1), Direction::Down);
			}else {
				player.movePlayer(0, 1);
			}
			if (player.decreasePhase() && !player.canPhase())onPhaseExit();
		}
		changer.key.code = sf::Keyboard::Key::Unknown;//implementing it here, because of handleItemSelect()
	}
	if (changer.key.code == sf::Keyboard::Key::Left) {
		if (player.get_x() > 0 &&
			tile_ids[player.get_x() - 1 + player.get_y() * tiles_width] != 1) {
			if (willFightEnemy(player.get_x() - 1, player.get_y())) {
				handleCombat(getEnemyThere(player.get_x() - 1, player.get_y()), Direction::Left);
			}else {
				player.movePlayer(-1, 0);
			}
			if (player.decreasePhase() && !player.canPhase())onPhaseExit();
		}
		changer.key.code = sf::Keyboard::Key::Unknown;//implementing it here, because of handleItemSelect()
	}
	if (changer.key.code == sf::Keyboard::Key::Right) {
		if (player.get_x() < tiles_width &&
			tile_ids[player.get_x() + 1 + player.get_y() * tiles_width] != 1) {
			if (willFightEnemy(player.get_x() + 1, player.get_y())) {
				handleCombat(getEnemyThere(player.get_x() + 1, player.get_y()), Direction::Right);
			}else {
				player.movePlayer(1, 0);
			}
			if(player.decreasePhase() && !player.canPhase())onPhaseExit();
		}
		changer.key.code = sf::Keyboard::Key::Unknown;//implementing it here, because of handleItemSelect()
	}
	
}
bool Game::willFightEnemy(unsigned x, unsigned y) {
	return !player.canPhase() && checkEnemyThere(x, y);
}
void Game::handleCombat(Enemy* foe, Direction direction){
	unsigned playerdamage = 15;
	ItemType usedItem = ItemType::Nothing;
	if (foe->get_ItemUsedOn() == ItemType::Nothing) {
		usedItem = onItemUse(playerdamage, direction);
		foe->set_ItemUsedOn(usedItem);
	}
	if (usedItem != ItemType::InvisiHat && usedItem != ItemType::PotionOfTeleport) {
		modifyDamage(foe, playerdamage);
		switch (foe->getType()) {
		case EnemyType::Cheshire:
			unsigned x, y;
			do {
				x = rand() % tiles_width;
				y = rand() % tiles_height;
			} while (!canTeleportThere(x, y));
			foe->takeDamage(playerdamage);
			if(foe->getAlive())player.set_position(x, y);
			break;
		case EnemyType::Hatter:
			foe->takeDamage(playerdamage);
			if (foe->getAlive())player.takeDamage(10 + rand() % 30);
			else {
				unsigned x, y;
				do {
					x = rand() % tiles_width;
					y = rand() % tiles_height;
				} while (!canTeleportThere(x, y));
				player.set_position(x, y);
			}
			break;
		case EnemyType::Queen:
			foe->takeDamage(playerdamage);
			if (foe->getAlive())player.takeDamage(20);
			break;
		case EnemyType::Rabbit:
			foe->takeDamage(playerdamage);
			if (foe->getAlive())player.takeDamage(25);
		}
	}
	lastEnemyHp = foe->get_hp();
	if (!player.getAlive())onPlayerDeath();
	return;
}

ItemType Game::onItemUse(unsigned& damage, Direction direction){
	if (selected_item >= 0 && selected_item <= 2) {
		ItemType item = player.getItem(selected_item);
		player.useItem(selected_item);
		int coinToss = rand() % 2;
		switch (item) {
		case ItemType::DrinkMe:
			damage = 10;
			selected_item = -1;
			return item;
		case ItemType::EatMe:
			damage = 40;
			selected_item = -1;
			return item;
		case ItemType::MagicFan:
			damage = 35;
			selected_item = -1;
			return item;
		case ItemType::Rose:
			(coinToss == 1) ? damage = 40 : damage = 10;
			selected_item = -1;
			return item;
		case ItemType::PotionOfTeleport:
			potionOfTeleport(direction);
			damage = 0;
			selected_item = -1;
			return item;
		case ItemType::InvisiHat:
			player.setPhaseFor(3);//because handleMovemend() immediately reduces the phase timer
			selected_item = -1;
			return item;
		}
	}
	selected_item = -1;
	return ItemType::Nothing;
}
void Game::modifyDamage(Enemy* foe, unsigned& damage) {
	int coinToss = rand() % 2;
	ItemType item = foe->get_ItemUsedOn();
	switch (item) {
	case ItemType::DrinkMe:
		damage = 10;
		break;
	case ItemType::EatMe:
		damage = 40;
		break;
	case ItemType::MagicFan:
		damage = 35;
		break;
	case ItemType::Rose:
		(coinToss == 1) ? damage = 40 : damage = 10;
		break;
	default:
		damage = 15;
	}
}
void Game::potionOfTeleport(Direction direction) {
	unsigned p_y = player.get_y();
	unsigned p_x = player.get_x();
	if (direction == Direction::Up) {
		for (unsigned i = 3; p_y - i >= 0; i++) {
			if (canTeleportThere(p_x, p_y - i)) {
				player.set_position(p_x, p_y - i);
				break;
			}
		}
	}
	else if (direction == Direction::Down) {
		for (unsigned i = 3; p_y + i < tiles_height; i++) {
			if (canTeleportThere(p_x, p_y + i)) {
				player.set_position(p_x, p_y + i);
				break;
			}
		}
	}
	else if (direction == Direction::Left) {
		for (unsigned i = 3; p_x - i >= 0; i++) {
			if (canTeleportThere(p_x - i, p_y)) {
				player.set_position(p_x - i, p_y);
				break;
			}
		}
	}else if(direction == Direction::Right){
		for (unsigned i = 3; p_x + i < tiles_width; i++) {
			if (canTeleportThere(p_x + i, p_y)) {
				player.set_position(p_x + i, p_y);
				break;
			}
		}
	}
}
void Game::onPhaseExit(){
	if (checkEnemyThere(player.get_x(), player.get_y())) {
		Enemy* foe = getEnemyThere(player.get_x(), player.get_y());
		while (player.getAlive() && foe->getAlive()) {
			foe->takeDamage(1);//suffocation damage
			player.takeDamage(1);//suffocation damage
		}
		if (!player.getAlive())onPlayerDeath();
	}
}

void Game::handleEventTiles(){
	//convert from matrix to array coordinates
	unsigned coords = this->player.get_x() + this->player.get_y() * this->tiles_width;
	switch (tile_ids[coords]) {
	case 3:
		//Is on the exit portal
		levelLoading();
		break;
	case 4://Potion of Teleport
		//if player has space in inventory to pick up the item
		if (this->player.addItem(ItemType::PotionOfTeleport)) {
			tile_ids[coords] = 0;//revert to path
		}
		break;
	case 5://DrinkMe
		if (this->player.addItem(ItemType::DrinkMe)) {
			tile_ids[coords] = 0;//revert to path
		}
		break;
	case 6://EatMe
		if (this->player.addItem(ItemType::EatMe)) {
			tile_ids[coords] = 0;//revert to path
		}
		break;
	case 7://MagicFan
		if (this->player.addItem(ItemType::MagicFan)) {
			tile_ids[coords] = 0;//revert to path
		}
		break;
	case 8://InvisiHat
		if (this->player.addItem(ItemType::InvisiHat)) {
			tile_ids[coords] = 0;//revert to path
		}
		break;
	case 9://Rose
		if (this->player.addItem(ItemType::Rose)) {
			tile_ids[coords] = 0;//revert to path
		}
		break;
	}
}

void Game::renderTiles(sf::RenderWindow* window){
	//Renders the textures for the tiles
	unsigned size = tiles_height * tiles_width;
	for (unsigned i = 0; i < size; i++) {
		if (tile_ids[i] < tiles_t.size()) {
			this->generate.setTexture(&tiles_t[tile_ids[i]]);
			this->generate.setPosition(sf::Vector2f(50 * (i % tiles_width), 50 * (i / tiles_height)));
		}
		window->draw(generate);
	}
}

void Game::renderEnemies() {
	unsigned size = this->enemies.size();
	for (unsigned i = 0; i < size; i++) {
		this->enemies[i].render(this->window);
	}
}

void Game::renderPlayerStats(sf::RenderWindow* window){
	sf::String playerStats{ "Alice's HP: " };
	playerStats = playerStats + std::to_string(player.get_hp()) + " / 100 ";
	playerStats = playerStats + "Equipped Item: " + player.getItemAsString(selected_item) + "\n";
	playerStats = playerStats + "Levels Remaining: " + std::to_string(levels);
	playerStats = playerStats + " Last enemy HP: " + std::to_string(lastEnemyHp) + " / 100";
	displayPlayerStats.setString(playerStats);
	displayPlayerStats.setPosition(sf::Vector2f(0.0f, 50.0f * tiles_height));

	window->draw(displayPlayerStats);
}
void Game::onPlayerDeath() {
	unsigned teleport_to = 0;
	unsigned size = tile_ids.size();
	for (unsigned i = 0; i < size; i++) {
		if (tile_ids[i] == 2) {
			teleport_to = i;
			break;
		}
	}
	player.set_hp(100);
	player.set_isAlive(1);
	player.set_position(teleport_to % tiles_width, teleport_to / tiles_height);
	return;
}

