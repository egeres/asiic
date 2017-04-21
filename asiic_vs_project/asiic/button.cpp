#include <SFML/Audio.hpp>
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
	event_name = event_clicked;
}

bool button_image::is_inside(sf::Vector2i input_vector)
{
	if (input_vector.x > pos.x && input_vector.x < pos.x + wh.x)
	{
		if (input_vector.y > pos.y && input_vector.y < pos.y + wh.y)
		{
			return true;
		}
	}
	return false;
}

navigation_bar::navigation_bar(sf::Vector2i input_pos, sf::Color input_color, int input_padding, int input_spacing, std::string input_alignment, std::string input_disposicion, sf::Sound input_sound)
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

	overlay_sound = input_sound;
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
			list_of_buttons[i]->pos = sf::Vector2i(pos_x, pos_y);
			pos_x += list_of_buttons[i]->wh.x;
			pos_x += spacing;
		}
	}

	background_rectangle.setPosition(pos.x - total_size_x / 2,pos.y);
	background_rectangle.setSize(sf::Vector2f(total_size_x, 60));
	wh = sf::Vector2i(total_size_x,60);

	
}

void navigation_bar::render(sf::RenderWindow& input_window, sf::Vector2i input_vector)
{

	/*
	input_window.draw(background_rectangle);

	for (int i = 0; i < list_of_buttons.size(); i++)
	{
		input_window.draw(list_of_buttons[i]->spr);
	}
	*/

	input_window.draw(background_rectangle);

	if (is_inside(input_vector))
	{
		bool in_any_of_them = false;

		for (int i = 0; i < list_of_buttons.size(); i++)
		{
			if (list_of_buttons[i]->is_inside(input_vector))
			{	
				list_of_buttons[i]->spr.setColor(sf::Color(120, 120, 120));
				input_window.draw(list_of_buttons[i]->spr);

				if (overlayed_button != list_of_buttons[i]->event_name) 
				{
					//std::cout << "blah |" << overlayed_button << "|" << list_of_buttons[i]->event_name << "\n";
					overlay_sound.play();
				}
				overlayed_button = list_of_buttons[i]->event_name;
				in_any_of_them = true;
			}
			else
			{
				list_of_buttons[i]->spr.setColor(sf::Color(100, 100, 100));
				input_window.draw(list_of_buttons[i]->spr);
			}
		}

		if (in_any_of_them == false) { overlayed_button = ""; }
	}
	else
	{

		overlayed_button = "";
		
		for (int i = 0; i < list_of_buttons.size(); i++)
		{
			list_of_buttons[i]->spr.setColor(sf::Color(100, 100, 100));
			input_window.draw(list_of_buttons[i]->spr);
		}
	}



}

bool navigation_bar::is_inside(sf::Vector2i input_vector)
{

	if (alignment == "left") {

	}

	if (alignment == "centered")
	{
		input_vector.x += wh.x/2;
	}

	if (input_vector.x > pos.x && input_vector.x < pos.x + wh.x)
	{
		if (input_vector.y > pos.y && input_vector.y < pos.y + wh.y)
		{
			return true;
		}
	}
	return false;
}

std::string navigation_bar::check_click(sf::Vector2i input_vector)
{

	std::string return_string;

	if (is_inside(input_vector))
	{
		return_string = "clicked the toolbar...";
		
		for (int i = 0; i < list_of_buttons.size(); i++)
		{
			if (list_of_buttons[i]->is_inside(input_vector))
			{	
				return_string =  list_of_buttons[i]->event_name;
			}
		}
	}

	return return_string;
}