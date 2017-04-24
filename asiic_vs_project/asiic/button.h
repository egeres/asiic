#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//holder of all the elements from below
class gui_element
{
public:
	bool active;
	sf::Vector2i pos;
	sf::Vector2i wh;
};

//button, but with an image
class button_image : public gui_element
{
public:
	button_image(sf::Vector2i, sf::Sprite, std::string);
	bool is_inside(sf::Vector2i);
	sf::Sprite  spr;
	std::string event_name;
};

//button, but with text
class button_text : public gui_element
{
public:
	button_text(sf::Vector2i, std::string, std::string, sf::Font&, sf::Color, int);
	bool is_inside(sf::Vector2i);
	void update();
	int padding;
	sf::Text    txt_obj;
	std::string str;
	std::string event_name;
	sf::RectangleShape background_rectangle;
};

//navigation bar but for sexy button_img type elements
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
	std::vector<button_image*> list_of_buttons;
	std::string alignment;  
	std::string disposicion;
	int padding;
	int spacing;
	std::string overlayed_button;
};

//navigation bar but for intelectual button_tex type elements
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
	std::vector<button_text*> list_of_buttons;
	std::string alignment;  
	std::string disposicion;
	int padding;
	int spacing;
	std::string overlayed_button;
};