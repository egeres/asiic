#include "button.h"
#include <string>
#include <iostream>

button::button(int in_x, int in_y, int in_width, int in_height, std::string in_text)
{
	x = in_x;
	y = in_y;
	width  = in_width;
	height = in_height;
	text = in_text;
}

bool button::is_inside(int in_x, int in_y)
{
	if (in_x > x && in_x < x + width)
	{
		if (in_y > y && in_y < y + height)
		{
			return true;
		}
	}
	return false;
}

button_image::button_image(sf::Vector2i input_pos, sf::Sprite input_sprite, std::string event_clicked)
{
	pos = input_pos;
	//wh  = input_sprite.getGlobalBounds();
	wh  = sf::Vector2i(input_sprite.getGlobalBounds().width, input_sprite.getGlobalBounds().height);
	spr = input_sprite;
}

navigation_bar::navigation_bar(sf::Vector2i input_pos, sf::Color input_color, int input_padding, int input_spacing, std::string input_alignment, std::string input_disposicion)
{
	pos = input_pos;
	padding = input_padding;
	spacing = input_spacing;

	background_rectangle.setOutlineThickness(input_padding);
	background_rectangle.setOutlineColor(input_color);
	background_rectangle.setFillColor(input_color);
	background_rectangle.setPosition(pos.x,pos.y);
	background_rectangle.setSize(sf::Vector2f(50, 100));

	alignment   = input_alignment;
	disposicion = input_disposicion;
}

void navigation_bar::update()
{
	
	background_rectangle.setSize(sf::Vector2f(50, 100));

	int total_size_x = 0;

	for (int i = 0; i < list_of_buttons.size(); i++)
	{
		total_size_x += list_of_buttons[i]->wh.x;
		total_size_x += spacing;
	}

	total_size_x -= spacing;


	if (alignment == "centered")
	{

		int pos_x = pos.x - total_size_x / 2;
		int pos_y = pos.y;

		for (int i = 0; i < list_of_buttons.size(); i++)
		{
			std::cout << " pos_x = " << pos_x;
			list_of_buttons[i]->spr.setPosition(sf::Vector2f(pos_x, pos_y));
			pos_x += list_of_buttons[i]->wh.x;
			pos_x += spacing;
		}
	}

	background_rectangle.setPosition(pos.x - total_size_x / 2,pos.y);
	background_rectangle.setSize(sf::Vector2f(total_size_x, 60));

	
}

void navigation_bar::render(sf::RenderWindow& input_window)
{
	input_window.draw(background_rectangle);

	for (int i = 0; i < list_of_buttons.size(); i++)
	{
		input_window.draw(list_of_buttons[i]->spr);
	} 
}