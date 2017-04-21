#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class button
{
public:
	button(int, int, int, int, std::string);
	bool is_inside(int, int);

	int x;
	int y;
	int width;
	int height;
	std::string text;
};

class button_image
{
public:
	button_image(sf::Vector2i, sf::Sprite, std::string);
	sf::Vector2i pos;
	sf::Vector2i wh;
	sf::Sprite   spr;
};

class navigation_bar
{
public:
	navigation_bar(sf::Vector2i, sf::Color, int, int, std::string, std::string);
	void render(sf::RenderWindow&);
	void update();

	sf::RectangleShape background_rectangle;
	sf::Vector2i pos;
	std::vector<button_image*> list_of_buttons;
	std::string alignment;  //vertical or horizontal
	std::string disposicion;// centered, left, right
	int padding;
	int spacing;
};

