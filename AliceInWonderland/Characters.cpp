#include "Characters.h"

Player::Player(unsigned x, unsigned y, std::string filename){
	if (filename != "") {
		this->set_position(x, y);
		this->shape.setPosition(sf::Vector2f(x * 50.0f, y * 50.0f));
		this->set_texture(filename);
	}
}

void Player::movePlayer(int x, int y){
	this->pos_x += x;
	this->pos_y += y;
	this->shape.setPosition(shape.getPosition()+sf::Vector2f(50*x,50*y));
	return;
}
