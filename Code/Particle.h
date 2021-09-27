#pragma once
#include	"stdafx.h"
#include	<windows.h>

using namespace sf;

class Particle {
	int time;
	LONG lifetime;
	float size;
	RectangleShape shape;
	LONG born_t;
	Color color;
	int type;
	Transform wtransform;

public:

	Particle(Texture *texp, Color color, int lt, float size, int type);
	bool Update();

	void setPosition(Vector2f pos) { shape.setPosition(pos); };
	void setRotation(float angle) { shape.setRotation(angle); };
	RectangleShape getVisual() { return shape; };

	Transform getTransf() { return wtransform; };
	int getType() { return type; };

};