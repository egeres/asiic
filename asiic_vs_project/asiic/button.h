#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

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
	bool is_inside(sf::Vector2i);
	sf::Vector2i pos;
	sf::Vector2i wh;
	sf::Sprite   spr;
	std::string event_name;
};

class navigation_bar
{
public:
	navigation_bar(sf::Vector2i, sf::Color, int, int, std::string, std::string, sf::Sound);
	void render(sf::RenderWindow&, sf::Vector2i);
	void update();
	bool is_inside(sf::Vector2i);
	std::string navigation_bar::check_click(sf::Vector2i);

	sf::Sound overlay_sound;
	sf::RectangleShape background_rectangle;
	sf::Vector2i pos;
	sf::Vector2i wh;
	std::vector<button_image*> list_of_buttons;
	std::string alignment;  //vertical or horizontal
	std::string disposicion;// centered, left, right
	int padding;
	int spacing;
	std::string overlayed_button;
};

