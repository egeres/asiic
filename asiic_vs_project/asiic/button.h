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






class gui_element
{
public:
	bool active;
	sf::Vector2i pos;
	sf::Vector2i wh;
};

class spacer : public gui_element
{
public:
	spacer(sf::Vector2i);
};

class button_image : public gui_element
{
public:
	button_image(sf::Vector2i, sf::Sprite, std::string);
	bool is_inside(sf::Vector2i);
	//sf::Vector2i pos;
	//sf::Vector2i wh;
	sf::Sprite  spr;
	std::string event_name;
};

class button_text : public gui_element
{
public:
	button_text(sf::Vector2i, std::string, std::string, sf::Font&, sf::Color, int);
	bool is_inside(sf::Vector2i);
	void update();
	//sf::Vector2i pos;
	//sf::Vector2i wh;
	int padding;
	sf::Text    txt_obj;
	std::string str;
	std::string event_name;
	sf::RectangleShape background_rectangle;
};

class navigation_bar_img : public gui_element
{
public:
	navigation_bar_img(sf::Vector2i, sf::Color, int, int, std::string, std::string, sf::Sound);
	void render(sf::RenderWindow&, sf::Vector2i);
	void update();
	bool is_inside(sf::Vector2i);
	std::string navigation_bar_img::check_click(sf::Vector2i);

	sf::Sound overlay_sound;
	sf::RectangleShape background_rectangle;
	//sf::Vector2i pos;
	//sf::Vector2i wh;
	std::vector<button_image*> list_of_buttons;
	//std::vector<gui_element*> list_of_buttons;
	std::string alignment;  //vertical or horizontal
	std::string disposicion;// centered, left, right
	int padding;
	int spacing;
	std::string overlayed_button;
};


class navigation_bar_txt : public gui_element
{
public:
	navigation_bar_txt(sf::Vector2i, sf::Color, int, int, std::string, std::string, sf::Sound);
	void render(sf::RenderWindow&, sf::Vector2i);
	void update();
	bool is_inside(sf::Vector2i);
	std::string check_click(sf::Vector2i);
	int index_by_click(sf::Vector2i);

	sf::Sound overlay_sound;
	sf::RectangleShape background_rectangle;
	//sf::Vector2i pos;
	//sf::Vector2i wh;
	std::vector<button_text*> list_of_buttons;
	//std::vector<gui_element*> list_of_buttons;
	std::string alignment;  //vertical or horizontal
	std::string disposicion;// centered, left, right
	int padding;
	int spacing;
	std::string overlayed_button;
};