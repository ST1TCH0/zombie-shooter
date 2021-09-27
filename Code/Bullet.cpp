#include	"Bullet.h"

Bullet::Bullet(float size) : currVelocity(0.f, 0.f), maxSpeed(35.f) {
	shape.setSize(Vector2f(size, size*3));
	shape.setFillColor(Color::Yellow);
}