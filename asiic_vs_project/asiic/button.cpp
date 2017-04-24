#include <SFML/Audio.hpp>
#include "button.h"
#include <string>
#include <iostream>



//constructor for the sexiest kind of button
button_image::button_image(sf::Vector2i input_pos, sf::Sprite input_sprite, std::string event_clicked)
{
	active = true;
	pos = input_pos;
	wh  = sf::Vector2i(input_sprite.getGlobalBounds().width, input_sprite.getGlobalBounds().height);
	spr = input_sprite;
	event_name = event_clicked;
	spr.setPosition(pos.x, pos.y);
}
//check if it's inside
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



//constructor for the smartest kind of button
button_text::button_text(sf::Vector2i input_pos, std::string display_string, std::string event_clicked, sf::Font& input_font, sf::Color input_color, int input_padding)
{
	padding = input_padding;
	active  = true;
	pos     = input_pos;
	str     = display_string;

	txt_obj.setFont(input_font);
	txt_obj.setCharacterSize(60);
	txt_obj.setColor(sf::Color::White);
	txt_obj.setString(str);
	txt_obj.setPosition(pos.x, pos.y);

	wh  = sf::Vector2i(txt_obj.getGlobalBounds().width +padding*2, txt_obj.getGlobalBounds().height+padding*2);

	background_rectangle.setFillColor(input_color);
	background_rectangle.setPosition(pos.x, pos.y);
	background_rectangle.setSize(sf::Vector2f(wh.x, wh.y));

	event_name = event_clicked;
}
//updater for this kind of button, because yeah, these buttons can change the text
void button_text::update()
{
	txt_obj.setString(str);
	wh  = sf::Vector2i(txt_obj.getGlobalBounds().width +padding*2, txt_obj.getGlobalBounds().height+padding*2);
	background_rectangle.setSize(sf::Vector2f(wh.x, wh.y));
}
//checks if the input coordinates go inside of the button
bool button_text::is_inside(sf::Vector2i input_vector)
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



//constructor for navigation_bar_img
navigation_bar_img::navigation_bar_img(sf::Vector2i input_pos, sf::Color input_color, int input_padding, int input_spacing, std::string input_alignment, std::string input_disposicion, sf::Sound input_sound)
{
	active = true;
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
//updater for the navigation_bar_img object
void navigation_bar_img::update()
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
//renders that navigation bar to a windows
void navigation_bar_img::render(sf::RenderWindow& input_window, sf::Vector2i input_vector)
{
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

				if (overlayed_button != list_of_buttons[i]->event_name && list_of_buttons[i]->event_name != "") 
				{
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
//checks if the inputted vector is inside of the navigation bar
bool navigation_bar_img::is_inside(sf::Vector2i input_vector)
{

	if (alignment == "left")
	{

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
//returns a string index which represents the click information
std::string navigation_bar_img::check_click(sf::Vector2i input_vector)
{

	std::string return_string;

	if (is_inside(input_vector))
	{
		return_string = "clicked the toolbar...";
		
		for (int i = 0; i < list_of_buttons.size(); i++)
		{
			if (list_of_buttons[i]->is_inside(input_vector) && list_of_buttons[i]->event_name != "")
			{	
				return_string =  list_of_buttons[i]->event_name;
			}
		}
	}

	return return_string;
}



//constructor for navigation_bar_txt
navigation_bar_txt::navigation_bar_txt(sf::Vector2i input_pos, sf::Color input_color, int input_padding, int input_spacing, std::string input_alignment, std::string input_disposicion, sf::Sound input_sound)
{
	active = true;
	pos = input_pos;
	padding = input_padding;
	spacing = input_spacing;

	background_rectangle.setOutlineThickness(input_padding);
	background_rectangle.setOutlineColor(input_color);
	background_rectangle.setFillColor(   input_color);
	background_rectangle.setPosition(pos.x,pos.y);
	background_rectangle.setSize(sf::Vector2f(50, 100));

	alignment   = input_alignment;
	disposicion = input_disposicion;

	overlay_sound = input_sound;
}
//updates the navigation_bar_txt bar
void navigation_bar_txt::update()
{
	background_rectangle.setSize(sf::Vector2f(50, 100));

	for (int i = 0; i < list_of_buttons.size(); i++)
	{
		list_of_buttons[i]->update();
	}

	int total_size_x = 0;
	int total_size_y = 0;

	for (int i = 0; i < list_of_buttons.size(); i++)
	{
		if (list_of_buttons[i]->wh.y > total_size_y) { total_size_y = list_of_buttons[i]->wh.y; }

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
			list_of_buttons[i]->pos = sf::Vector2i(pos_x, pos_y);
			list_of_buttons[i]->background_rectangle.setPosition(pos_x, pos_y);
			list_of_buttons[i]->txt_obj.setPosition(pos_x + list_of_buttons[i]->padding, pos_y - 33 + list_of_buttons[i]->padding);

			pos_x += list_of_buttons[i]->wh.x;
			pos_x += spacing;
		}
	}

	background_rectangle.setPosition(pos.x - total_size_x / 2,pos.y);
	background_rectangle.setSize(sf::Vector2f(total_size_x, total_size_y));
	wh = sf::Vector2i(total_size_x,total_size_y);
}
//navigation_bar_txt's renderer
void navigation_bar_txt::render(sf::RenderWindow& input_window, sf::Vector2i input_vector)
{
	input_window.draw(background_rectangle);

	if (is_inside(input_vector))
	{
		bool in_any_of_them = false;

		for (int i = 0; i < list_of_buttons.size(); i++)
		{
			if (list_of_buttons[i]->is_inside(input_vector))
			{	
				list_of_buttons[i]->txt_obj.setColor(sf::Color(100, 100, 100));
				input_window.draw(list_of_buttons[i]->background_rectangle);
				input_window.draw(list_of_buttons[i]->txt_obj);

				if (overlayed_button != list_of_buttons[i]->event_name && list_of_buttons[i]->event_name != "") 
				{
					overlay_sound.play();
				}
				overlayed_button = list_of_buttons[i]->event_name;
				in_any_of_them = true;
			}
			else
			{
				list_of_buttons[i]->txt_obj.setColor(sf::Color(50, 50, 50));
				input_window.draw(list_of_buttons[i]->background_rectangle);
				input_window.draw(list_of_buttons[i]->txt_obj);
			}
		}

		if (in_any_of_them == false) { overlayed_button = ""; }
	}
	else
	{
		overlayed_button = "";

		for (int i = 0; i < list_of_buttons.size(); i++)
		{
			list_of_buttons[i]->txt_obj.setColor(sf::Color(50, 50, 50));
			input_window.draw(list_of_buttons[i]->background_rectangle);
			input_window.draw(list_of_buttons[i]->txt_obj);
		}
	}
}
//check's if the given vector position is inside of navigation_bar_txt object
bool navigation_bar_txt::is_inside(sf::Vector2i input_vector)
{
	if (alignment == "left")
	{
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
//check's which element of the bar has been clicked by text
std::string navigation_bar_txt::check_click(sf::Vector2i input_vector)
{
	std::string return_string;
	if (is_inside(input_vector))
	{
		return_string = "clicked the toolbar...";
		for (int i = 0; i < list_of_buttons.size(); i++)
		{
			if (list_of_buttons[i]->is_inside(input_vector) && list_of_buttons[i]->event_name != "")
			{	
				return_string =  list_of_buttons[i]->event_name;
			}
		}
	}
	return return_string;
}
//check's which element of the bar has been clicked by index
int navigation_bar_txt::index_by_click(sf::Vector2i input_vector)
{

	int return_string = -1;
	if (is_inside(input_vector))
	{
		for (int i = 0; i < list_of_buttons.size(); i++)
		{
			if (list_of_buttons[i]->is_inside(input_vector) && list_of_buttons[i]->event_name != "")
			{	
				return_string = i;
			}
		}
	}

	return return_string;
}