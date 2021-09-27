#include "Render.h"

Render::Render(RenderWindow* window) { 
	this->window = window; 
	waitb = Texture();
	waitb.loadFromFile("Textures/waiting.jpg");
	if (!wait.loadFromFile("Textures\\wait.png")) printf("ERROR: unexpected texture");
	if (!texb.loadFromFile("Textures\\backgroundnew.jpg")) printf("ERROR: unexpected texture");
	if (!texp.loadFromFile("Textures\\player.png")) printf("ERROR: unexpected texture");
	if (!texz.loadFromFile("Textures\\zombie.png")) printf("ERROR: unexpected texture");
	if (!texbl.loadFromFile("Textures\\blood.png")) printf("ERROR: unexpected texture");
	if (!texf.loadFromFile("Textures\\fire.png")) printf("ERROR: unexpected texture");
	if (!texsm.loadFromFile("Textures\\smoke.png")) printf("ERROR: unexpected texture");
	if (!texl.loadFromFile("Textures\\light.png")) printf("ERROR: unexpected texture");
	font.loadFromFile("Fonts/PixellettersFull.ttf");
	waitingb.setTexture(waitb);
	waiting.setTexture(wait);
	background.setTexture(texb);
}

Texture& Render::GetTexture(std::string name) {
	if (name == "player") return texp;
	else if (name == "zombie") return texz;
	else if (name == "blood") return texbl;
	else if (name == "fire") return texf;
	else if (name == "smoke") return texsm;
	else if (name == "light") return texl;
}

Font Render::GetFont() { return font; }

void Render::RenderStatic() {
	window->clear();
	window->draw(background);
}

void Render::LoadTextures() {
	
}

void Render::UIWait() {
	waiting.setPosition(1600 / 2 - 150, 900 / 2 - 150);
	window->clear();
	window->draw(waitingb);
	wtransform.rotate(2, Vector2f(1600 / 2, 900 / 2));
	window->draw(waiting, wtransform);
	window->display();
}