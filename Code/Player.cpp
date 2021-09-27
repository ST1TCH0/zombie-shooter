#include	"Player.h"

Player::Player() {

}

Player::Player(float size, Vector2f p, std::string name) {
	shape = new RectangleShape();
	shape->setSize(Vector2f(size, size));
	aim = Vector2f(0, 0);
	pos = p;
	health = 100;
	nickname = name;
	dir = 0;
	is_dead = 0;
	score = 0;
	click = false;
}

void Player::Update() {

	if (health < 1) is_dead = 1;

	shape->setRotation(r_angle);
	Vector2f a(25.0 / sqrt(2) * cos(3.1415 / 4 + r_angle / 180 * 3.1415), 25.0 / sqrt(2) * sin(3.1415 / 4 + r_angle / 180 * 3.1415));
	shape->setPosition(pos - a);
}

Player::~Player() {

}
