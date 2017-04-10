#include <SFML/Graphics.hpp>
#include "canvas.h"
#include <iostream>	
#include <string> 

void draw_grid(sf::RenderWindow& input_window, canvas input_canvas, int disp_x, int disp_y, int spacing)
{

	sf::RectangleShape rectangle(sf::Vector2f(spacing, spacing));
	rectangle.setFillColor(sf::Color(5, 5, 5));
	rectangle.setOutlineThickness(1);
	rectangle.setOutlineColor(sf::Color(10, 10, 10));

	for (int i = 0; i < input_canvas.size_x; i++) {
		for (int j = 0; j < input_canvas.size_y; j++) {

			rectangle.setPosition(disp_x + i * spacing, disp_y + j * spacing);
			input_window.draw(rectangle);

		}
	}
}

void draw_selected(sf::RenderWindow& input_window, canvas input_canvas, int disp_x, int disp_y, int spacing)
{

	sf::RectangleShape rectangle(sf::Vector2f(spacing, spacing));
	rectangle.setFillColor(sf::Color(20, 20, 20));
	//rectangle.setOutlineThickness(1);
	//rectangle.setOutlineColor(sf::Color(100, 100, 100));

	for (int i = 0; i < input_canvas.size_x; i++) {
		for (int j = 0; j < input_canvas.size_y; j++) {

			if (input_canvas.activ_cells[i][j]) {
				rectangle.setPosition(disp_x + i * spacing, disp_y + j * spacing);
				input_window.draw(rectangle);
			}

		}
	}
}

void draw_characters(sf::RenderWindow& input_window, canvas input_canvas, int disp_x, int disp_y, int spacing, sf::Text& text_obj)
{

	sf::RectangleShape rectangle(sf::Vector2f(spacing, spacing));
	rectangle.setFillColor(sf::Color(20, 20, 20));
	//rectangle.setOutlineThickness(1);
	//rectangle.setOutlineColor(sf::Color(100, 100, 100));

	for (int i = 0; i < input_canvas.size_x; i++) {
		for (int j = 0; j < input_canvas.size_y; j++) {

			text_obj.setPosition(disp_x + i * spacing, disp_y + j * spacing);
			text_obj.setString(input_canvas.cell_letters[i][j]);
			input_window.draw(text_obj);

		}
	}
}


bool inside_rect(int in_x, int in_y, int pos_x, int pos_y, int width, int height) {

	if (in_x > pos_x && in_x < pos_x + width) 
	{
		if (in_y > pos_y && in_y < pos_y + height) 
		{
			return true;
		}
	}
	return false;
}

sf::Vector2i cell_location(sf::Vector2i inpt, canvas input_canvas, int spacing) {


	sf::Vector2i to_return;

	int offset_x;
	offset_x     = inpt.x % spacing;
	offset_x     = inpt.x - offset_x;
	offset_x     = offset_x / spacing;

	int offset_y;
	offset_y     = inpt.y % spacing;
	offset_y     = inpt.y - offset_y;
	offset_y     = offset_y / spacing;

	to_return.x = offset_x;
	to_return.y = offset_y;

	return to_return;
}

int main()
{
	using namespace std;
	sf::RenderWindow window(sf::VideoMode(1500, 900), "ASIIC editor");

	int displacement_x = 50;
	int displacement_y = 50;
	int cell_size      = 40;

	canvas new_canvas = canvas(20, 20);

	new_canvas.activ_cells[3][4] = true;
	new_canvas.cell_letters[3][4] = 'e';
	new_canvas.cell_letters[3][5] = 'e';
	new_canvas.cell_letters[3][6] = 'e';

	bool out_of_canvas = true;

	bool mouse_button_down;
	bool prev_mouse_button_down;

	sf::Vector2i mouse_position;
	sf::Vector2i prev_mouse_position;
	sf::Vector2i cell_location_vector;

	sf::Vector2i displacement_v;


	sf::Font font;
	if (!font.loadFromFile("consolas.ttf"))
	{
	    // error...
	}

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(50);
	text.setColor(sf::Color::White);


	while (window.isOpen())
	{

		//update variables
		mouse_position       = sf::Mouse::getPosition(window);
		out_of_canvas        = !inside_rect( mouse_position.x, mouse_position.y, displacement_x, + displacement_y, 20*cell_size, 20*cell_size);
		displacement_v.x     = displacement_x;
		displacement_v.y     = displacement_y;
		cell_location_vector = cell_location(mouse_position - displacement_v, new_canvas, cell_size);

		mouse_button_down    = sf::Mouse::isButtonPressed(sf::Mouse::Left);

		//loop itself
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (mouse_button_down) // && !prev_mouse_button_down)
		{
			if (out_of_canvas)
			{
				displacement_x += mouse_position.x - prev_mouse_position.x;
				displacement_y += mouse_position.y - prev_mouse_position.y;
			}

			else 
			{
				new_canvas.activ_cells[cell_location_vector.x][cell_location_vector.y] = true;
				cout << std::to_string(cell_location_vector.x) << "  "<< std::to_string(cell_location_vector.y) << "  :";
			}
			
		}

		if (event.type == sf::Event::TextEntered) {
			if (event.text.unicode < 256) {
				new_canvas.set_char_selected(event.text.unicode);
				new_canvas.deselect_all();
			}
		}


		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{

				new_canvas.deselect_all();

				/*
				std::cout << "the escape key was pressed" << std::endl;
				std::cout << "control:" << event.key.control << std::endl;
				std::cout << "alt:" << event.key.alt << std::endl;
				std::cout << "shift:" << event.key.shift << std::endl;
				std::cout << "system:" << event.key.system << std::endl;ç
				*/
			
		}

		window.clear();

		window.draw(text);

		draw_grid(      window, new_canvas, displacement_x, displacement_y, cell_size);
		draw_selected(  window, new_canvas, displacement_x, displacement_y, cell_size);
		draw_characters(window, new_canvas, displacement_x, displacement_y, cell_size, text);

		window.display();

		//prev variables
		prev_mouse_position    = mouse_position;
		prev_mouse_button_down = mouse_button_down;
	}

	return 0;
}


