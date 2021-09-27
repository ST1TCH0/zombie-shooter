#include	"Particle.h"

Particle::Particle(Texture *texp, Color c , int lt, float s, int t) {

	shape.setSize(Vector2f(s, s));
	shape.setTexture(texp);
	//shape.setFillColor(c);
	color = c;
	if (time >= 255) time = 254;
	time = lt;
	size = s;
	type = t;
}

bool Particle::Update() {

	/*SYSTEMTIME time;
	GetSystemTime(&time);
	LONG time_ms = (time.wSecond * 1000) + time.wMilliseconds;

	shape.setFillColor(Color(color.r, color.g, color.b, ((float)(((born_t + lifetime) - time_ms) / (float)lifetime) * 254.0)));
	
	if ((born_t + lifetime) < time_ms) return false;*/
	
	if (type == 1) {
		shape.move(Vector2f((-1 + (rand() % 2)) * 2, (-1 + (rand() % 2)) * 2));
		wtransform.rotate(1, Vector2f(shape.getPosition().x + size / 2, shape.getPosition().y + size / 2));
	}
	if (type == 2) {
		wtransform.rotate(-10 + (rand() % 20), Vector2f(shape.getPosition().x + size / 2, shape.getPosition().y + size / 2));
	}
	if (color.a > 0 && type != 2) shape.setFillColor(Color(color.r, color.g, color.b, (float)time));
	time--;
	if (time < 0)return false;
	return true;

}