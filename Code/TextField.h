#pragma once

#ifndef TXTFIELD_H
#define TXTFIELD_H

#include <string>

#include <SFML/Graphics.hpp>


const int GUI_TEXT_MAX = 24;

const int GUI_TEXT_BACKSPACE = 8;
const int GUI_TEXT_ESCAPE = 27;

const sf::Color GUI_TEXT_GRAY = sf::Color(105, 105, 105);

class TextField {
public:
	TextField();

	void setPosition(sf::Vector2f vec);

	void input(sf::Event ev);

	void setFont(sf::Font& f);

	const sf::String& getText();

	void setPlaceholder(std::string str);

	void render(sf::RenderWindow& window);

	void setLength(int arg);

	int getTextLength();

	bool isActive();

	void open();

	void setActive(bool arg);
private:
	sf::Text txt;
	sf::RectangleShape box;

	int size;

	int length;

	bool renderPlaceholder;

	

	bool active;

	sf::Vector2f pos;

	std::string placeholder;
};

#endif