#pragma once
enum class EnemyType{ Cheshire, Queen, Rabbit, Hatter};
enum class ItemType { PotionOfTeleport, DrinkMe, EatMe, MagicFan, InvisiHat, Rose, Nothing};
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
class Characters{
protected:
	unsigned hp = 100;
	unsigned pos_x;
	unsigned pos_y;
	sf::Texture texture;
	sf::RectangleShape shape;
	bool isAlive = 1;
public:
	Characters(unsigned x=0, unsigned y=0, std::string filename= "") : pos_x{ x }, pos_y{y} {
		shape.setSize(sf::Vector2f(50.0f, 50.0f));
		if (filename != "") {
			set_texture(filename);
			shape.setTexture(&texture);
			shape.setPosition(sf::Vector2f(x * 50.0f, y * 50.0f));
		}
	};
	void takeDamage(unsigned damage) {
		if (hp <= damage) {
			hp = 0;
			isAlive = false;
			return;
		}
		hp -= damage;
	}
	bool getAlive()const {
		return isAlive;
	}
	sf::RectangleShape getShape()const {
		return shape;
	}
	unsigned get_x()const {
		return pos_x;
	}
	unsigned get_y()const {
		return pos_y;
	}
	unsigned get_hp()const {
		return hp;
	}
	void set_hp(unsigned health) {
		hp = health;
	}
	void set_isAlive(bool alive) {
		isAlive = alive;
	}
	void set_x(unsigned x) {
		this->pos_x = x;
	}
	void set_y(unsigned y) {
		this->pos_y = y;
	}
	void set_position(unsigned x, unsigned y) {
		this->set_x(x);
		this->set_y(y);
		this->shape.setPosition(sf::Vector2f(x * 50.0f, y * 50.0f));
	}
	void set_texture(std::string filename) {
		this->texture.loadFromFile(filename);
		this->shape.setTexture(&texture);
	}
	
};
class Player :public Characters {
private:
	unsigned canPhaseFor = 0;
	ItemType inventory[3]{ ItemType::Nothing };
public:
	Player(unsigned x = 0, unsigned y = 0, std::string filename = "") {
		if (filename != "") {
			this->set_position(x, y);
			this->shape.setPosition(sf::Vector2f(x * 50.0f, y * 50.0f));
			this->set_texture(filename);
		}
	}
	void setPhaseFor(unsigned turns) {
		canPhaseFor = turns;
	}
	void movePlayer(int x, int y) {
		this->pos_x += x;
		this->pos_y += y;
		this->shape.setPosition(shape.getPosition() + sf::Vector2f(50 * x, 50 * y));
		return;
	}
	bool canPhase()const {
		return canPhaseFor != 0;
	}
	bool decreasePhase() {
		if (canPhase()) {
			canPhaseFor--;
			return true;
		}
		return false;
	}
	unsigned getPhase()const {
		return canPhaseFor;
	}
	ItemType getItem(int invSlot)const {
		if (invSlot >= 3 || invSlot < 0)return ItemType::Nothing;
		return inventory[invSlot];
	}
	sf::String getItemAsString(int invSlot)const {
		if (invSlot >= 3 || invSlot < 0)return "Nothing";
		sf::String item;
		switch (inventory[invSlot]){
		case ItemType::PotionOfTeleport:
			item = "Potion Of Teleportation";
			break;
		case ItemType::DrinkMe:
			item = "Drink Me";
			break;
		case ItemType::EatMe:
			item = "Eat Me";
			break;
		case ItemType::MagicFan:
			item = "Magic Fan";
			break;
		case ItemType::InvisiHat:
			item = "Invisible Hat";
			break;
		case ItemType::Rose:
			item = "White Red Rose";
			break;
		default:
			item = "Nothing";
		}
		return item;
	}
	bool useItem(int invSlot) {
		if (invSlot >= 3 || invSlot < 0 || inventory[invSlot] == ItemType::Nothing)return false;
		inventory[invSlot] = ItemType::Nothing;
		return true;
	}
	bool addItem(ItemType item) {
		for (unsigned i = 0; i < 3; i++) {
			if (inventory[i] == ItemType::Nothing) {
				inventory[i] = item;
				return true;
			}
		}
		return false;
	}
	void clearInventory() {
		for (unsigned i = 0; i < 3; i++) {
			inventory[i] = ItemType::Nothing;
		}
	}
	void render(sf::RenderWindow* window) {
		if (isAlive) {
			set_texture("../Textures/alice.png");
			window->draw(shape);
		}
	}
};
class Enemy : public Characters {
private:
	EnemyType type;
	ItemType itemUsedOn=ItemType::Nothing;
public:
	Enemy(EnemyType type=EnemyType::Rabbit, unsigned x=0, unsigned y=0){
		this->type = type;
		set_position(x, y);
	}
	EnemyType getType()const {
		return type;
	}
	ItemType get_ItemUsedOn()const{
		return itemUsedOn;
	}
	void set_ItemUsedOn(ItemType type) {
		itemUsedOn = type;
	}
	void render(sf::RenderWindow* window) {
		if (isAlive) {
			switch (type) {
			case EnemyType::Cheshire:
				set_texture("../Textures/cheshire.png");
				break;
			case EnemyType::Queen:
				set_texture("../Textures/queen.png");
				break;
			case EnemyType::Hatter:
				set_texture("../Textures/hatter.png");
				break;
			case EnemyType::Rabbit:
				set_texture("../Textures/rabbit.png");
				break;
			}

			window->draw(shape);
		}
	}
};

