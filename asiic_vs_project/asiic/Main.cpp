#include <SFML/Graphics.hpp>
#include "canvas.h"
#include <iostream>	
#include <string> 

void draw_grid(sf::RenderWindow& input_window, canvas input_canvas, int disp_x, int disp_y, int spacing)
{

	sf::RectangleShape rectangle(sf::Vector2f(spacing, spacing));
	rectangle.setFillColor(sf::Color(0, 0, 0));
	//rectangle.setOutlineThickness(1);
	//rectangle.setOutlineColor(sf::Color(60, 60, 60));

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

	bool out_of_canvas = true;

	sf::Vector2i mouse_position;
	sf::Vector2i prev_mouse_position;
	sf::Vector2i cell_location_vector;

	sf::Vector2i displacement_v;

	while (window.isOpen())
	{

		//update variables
		mouse_position       = sf::Mouse::getPosition(window);
		out_of_canvas        = !inside_rect( mouse_position.x, mouse_position.y, displacement_x, + displacement_y, 20*cell_size, 20*cell_size);
		displacement_v.x     = displacement_x;
		displacement_v.y     = displacement_y;
		cell_location_vector = cell_location(mouse_position - displacement_v, new_canvas, cell_size);

		//loop itself
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
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

		window.clear();
		draw_grid(    window, new_canvas, displacement_x, displacement_y, cell_size);
		draw_selected(window, new_canvas, displacement_x, displacement_y, cell_size);
		window.display();

		//prev variables
		prev_mouse_position = mouse_position;
	}

	return 0;
}


