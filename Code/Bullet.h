#pragma once
#include	"stdafx.h"

using namespace sf;

class Bullet {

public:

	RectangleShape shape;
	Vector2f currVelocity;
	float maxSpeed;


	Bullet(float size = 3.f);
};