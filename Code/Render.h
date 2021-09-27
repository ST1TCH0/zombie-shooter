#pragma once

#include	"stdafx.h"


class Render {
private:
	Texture waitb;
	Texture wait;
	Texture texb;
	Texture texp;
	Texture texz;
	Texture texbl;
	Texture texf;
	Texture texsm;
	Texture texl;

	Sprite waitingb;
	Sprite waiting;

	Transform wtransform;

	Font font;
	
	Sprite background;

	RenderWindow* window;

public:

	Render(RenderWindow* window);

	Texture& GetTexture(std::string name); 

	Font GetFont();

	void RenderStatic();

	void LoadTextures();

	void UIWait();
};