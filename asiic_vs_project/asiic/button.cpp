#include <SFML/Audio.hpp>
#include "button.h"
#include <string>
#include <iostream>







spacer::spacer(sf::Vector2i input_width)
{
	active = true;
	pos = sf::Vector2i(1,1);
	wh = input_width;
}







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
	active = true;
	pos = input_pos;
	//wh  = input_sprite.getGlobalBounds();
	wh  = sf::Vector2i(input_sprite.getGlobalBounds().width, input_sprite.getGlobalBounds().height);
	spr = input_sprite;
	event_name = event_clicked;
	spr.setPosition(pos.x, pos.y);
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







button_text::button_text(sf::Vector2i input_pos, std::string display_string, std::string event_clicked, sf::Font& input_font, sf::Color input_color, int input_padding)
{
	padding = input_padding;

	active = true;
	pos = input_pos;
	//wh  = input_sprite.getGlobalBounds();

	str = display_string;

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
	//spr.setPosition(pos.x, pos.y);
}

void button_text::update()
{
	txt_obj.setString(str);
	wh  = sf::Vector2i(txt_obj.getGlobalBounds().width +padding*2, txt_obj.getGlobalBounds().height+padding*2);
	background_rectangle.setSize(sf::Vector2f(wh.x, wh.y));
}

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

void navigation_bar_img::update()
{
	//std::cout << "updating img_bar... \n";

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

void navigation_bar_img::render(sf::RenderWindow& input_window, sf::Vector2i input_vector)
{

	//std::cout << "rendering img_bar... \n";

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

				if (overlayed_button != list_of_buttons[i]->event_name && list_of_buttons[i]->event_name != "") 
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

bool navigation_bar_img::is_inside(sf::Vector2i input_vector)
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







navigation_bar_txt::navigation_bar_txt(sf::Vector2i input_pos, sf::Color input_color, int input_padding, int input_spacing, std::string input_alignment, std::string input_disposicion, sf::Sound input_sound)
{
	//std::cout << "declaring txt_bar... \n";

	active = true;
	pos = input_pos;
	padding = input_padding;
	spacing = input_spacing;

	background_rectangle.setOutlineThickness(input_padding);
	//background_rectangle.setOutlineColor(sf::Color(55,66,250));
	//background_rectangle.setFillColor(sf::Color(220,66,55));
	background_rectangle.setOutlineColor(input_color);
	background_rectangle.setFillColor(   input_color);

	background_rectangle.setPosition(pos.x,pos.y);
	background_rectangle.setSize(sf::Vector2f(50, 100));

	alignment   = input_alignment;
	disposicion = input_disposicion;

	overlay_sound = input_sound;
}

void navigation_bar_txt::update()
{
	//std::cout << "updating txt_bar... \n";
	
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
			//std::cout << " pos_x = " << pos_x;
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

void navigation_bar_txt::render(sf::RenderWindow& input_window, sf::Vector2i input_vector)
{
	//std::cout << "rendering txt_bar 0... \n";

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
		//std::cout << "rendering txt_bar a... \n";

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
					//std::cout << "blah |" << overlayed_button << "|" << list_of_buttons[i]->event_name << "\n";
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

		//std::cout << "rendering txt_bar b... \n";

		overlayed_button = "";

		//std::cout << "rendering txt_bar 0... \n";

		for (int i = 0; i < list_of_buttons.size(); i++)
		{
			//std::cout << "drawing this :" << i << " \n";

			//std::cout << " a:" << list_of_buttons[i]->txt_obj.getPosition().x << " ; " << list_of_buttons[i]->txt_obj.getPosition().y << "\n";
			//std::cout << " b:" << list_of_buttons[i]->txt_obj.getString().toAnsiString()   << "\n";
			//std::cout << " a:" <<  << "\n";

			list_of_buttons[i]->txt_obj.setColor(sf::Color(50, 50, 50));

			//std::cout << input_window.
			input_window.draw(list_of_buttons[i]->background_rectangle);
			input_window.draw(list_of_buttons[i]->txt_obj);
			//std::cout << "exiting" << "\n";
		}
	}

	
	//std::cout << "exiting render txt bar";
}

bool navigation_bar_txt::is_inside(sf::Vector2i input_vector)
{

	//std::cout << "cheking if is inside txt_bar... \n";

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
			//std::cout << "exit \n";
			return true;
		}
	}

	//std::cout << "exit \n";
	return false;
}

std::string navigation_bar_txt::check_click(sf::Vector2i input_vector)
{

	//std::cout << "checking click txt_bar... \n";

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

int navigation_bar_txt::index_by_click(sf::Vector2i input_vector)
{

	//std::cout << "checking click txt_bar... \n";

	int return_string = -1;

	if (is_inside(input_vector))
	{
		//return_string = "clicked the toolbar...";
		
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