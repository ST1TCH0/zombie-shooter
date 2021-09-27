#pragma once
#include	"stdafx.h"

using namespace sf;

class Player {

private:

	float health;
	std::string nickname;
	Text namevisual;
	bool is_dead, click;
	int score, dir;
	RectangleShape *shape;
	Vector2f pos, aim;
	float r_angle;
	Transform transform;

public:
	Player();
	Player(float size, Vector2f pos, std::string nickname);
	virtual ~Player();

	void Update();

	RectangleShape* getVisual() { return shape; };

	void setHealth(float n) { if (health > 0) health = n; };
	float getHealth() { return health; };

	void setNickname(std::string name) {namevisual.setString(name); nickname = name;};
	std::string getNickname() { return nickname; };

	Text getVisualName() {return namevisual;};

	bool isDead() { return is_dead; };
	void setDead(bool val) { is_dead = val; };

	void addScore() { score++; };
	void setScore(int val) { score = val; };
	int getScore() { return score; };

	void setPosition(Vector2f n) { pos = n; };
	Vector2f getPosition() { return pos; };

	void setRotation(float n) { r_angle = n; };
	float getRotation() { return r_angle; };

	void setDir(int n) { dir = n; };
	int getDir() { return dir; };
	
	void setAim(Vector2f n) { aim = n; };
	Vector2f getAim() { return aim; };

	void setClick(bool n) { click = n; };
	bool getClick() { return click; };

	void move(float x, float y) { pos.x = pos.x + x; pos.y = pos.y + y; };

	Transform getTrans() { return transform; };

};