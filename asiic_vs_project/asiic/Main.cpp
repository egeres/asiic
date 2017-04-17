#include <SFML/Graphics.hpp>
#include "canvas.h"
#include "button.h"
#include <iostream>	
#include <string> 


/*  TO DO

	add debug mode
	find cool use for the enter key
	button system
	define button events properly
	
	functionality / tools :
		add ctrl+z
		bucket selection
		resize canvas dynamically
		"select by color"
		move selection with middle mouse ?
		blinking cursor displace text sideways both left and right
		move selection with arrow keys

	menu / interface :
		tool bar ?
		help window ?
		blinking cursor
		invert colors ?
		icon for the project
		right click pie menu
		show the "pixel" position

	bugs / improvements:
		fix square selection lagging
		fix "out of matrix" tier issues with the "blinking cursor"
		better text input, keys lag by some reason

*/


/* IMPLENTED / FIXED
	wand selection
	fix issue with pencil selection crashing w/ just 1 click
	backspace functionality implemented
	type-in as normal text implemented
	correct mouse in view
	better movement
	save to .txt
	pencil toggle selection
	square selection
	text size corrected
*/

//function which draws the base grid
void draw_grid(sf::RenderWindow& input_window, canvas input_canvas, int disp_x, int disp_y, int spacing_x, int spacing_y)
{
	sf::RectangleShape rectangle(sf::Vector2f(spacing_x, spacing_y));
	rectangle.setFillColor(sf::Color(5, 5, 5));
	rectangle.setOutlineThickness(1);
	rectangle.setOutlineColor(sf::Color(10, 10, 10));
	for (int i = 0; i < input_canvas.size_x; i++)
	{
		for (int j = 0; j < input_canvas.size_y; j++)
		{
			rectangle.setPosition(disp_x + i * spacing_x, disp_y + j * spacing_y);
			input_window.draw(rectangle);
		}
	}
}

//function which draws the selected cells in the grid
void draw_selected(sf::RenderWindow& input_window, canvas input_canvas, int disp_x, int disp_y, int spacing_x, int spacing_y)
{
	input_canvas.overlay_short_matrix(input_canvas.tmp_activ_cells);
	sf::RectangleShape rectangle(sf::Vector2f(spacing_x, spacing_y));
	rectangle.setFillColor(sf::Color(20, 20, 20));
	//rectangle.setOutlineThickness(1);
	//rectangle.setOutlineColor(sf::Color(100, 100, 100));

	//std::cout << "input_canvas_size " << input_canvas.size_x << "\n";
	//std::cout << "this cell...      " << activ_cells[0].size()activ_cells[200][0] << "\n";
	//std::cout << "dimensions are : " << input_canvas.activ_cells[0].size() << " by " << input_canvas.activ_cells.size();

	//for (int i = 0; i < input_canvas.size_y; i++)
	//{
	//	for (int j = 0; j < input_canvas.size_x; j++)
	//	{

	for (int i = 0; i < input_canvas.activ_cells.size(); i++)
	{
		for (int j = 0; j < input_canvas.activ_cells[0].size(); j++)
		{
			//std::cout << " x" << j << " y" << i << "|   ";

			if (input_canvas.activ_cells[i][j]) {

				rectangle.setPosition(disp_x+j*spacing_x, disp_y+i*spacing_y);

				input_window.draw(rectangle);
			}

		}
	}

	//std::cout << "i'm finished !\n";
}

//function to draw the characters from the matrix
void draw_characters(sf::RenderWindow& input_window, canvas input_canvas, int disp_x, int disp_y, int spacing_x, int spacing_y, sf::Text& text_obj)
{
	sf::RectangleShape rectangle(sf::Vector2f(spacing_x, spacing_y));
	rectangle.setFillColor(sf::Color(20, 20, 20));
	//rectangle.setOutlineThickness(1);
	//rectangle.setOutlineColor(sf::Color(100, 100, 100));

	//for (int i = 0; i < input_canvas.size_y; i++) 
	//{
	//	for (int j = 0; j < input_canvas.size_x; j++) 
	//	{

	for (int i = 0; i < input_canvas.cell_letters.size(); i++)
	{
		for (int j = 0; j < input_canvas.cell_letters[0].size(); j++)
		{
			text_obj.setPosition(disp_x + j * spacing_x - 10, disp_y + i * spacing_y);
			text_obj.setString(input_canvas.cell_letters[i][j]);
			input_window.draw(text_obj);
		}
	}
}

//function to draw the GUI buttons
void draw_buttons(sf::RenderWindow& input_window, canvas input_canvas, std::vector<button*> in_list_of_buttons, sf::Font font)
{
	//define text
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setColor(sf::Color::White);
	
	for (int i = 0; i < in_list_of_buttons.size(); i++)
	{
		//draw the base rectangle
		sf::RectangleShape rectangle(sf::Vector2f(in_list_of_buttons[i]->width, in_list_of_buttons[i]->height));
		rectangle.setFillColor(      sf::Color(80, 80, 80)         );
		rectangle.setPosition(in_list_of_buttons[i]->x, in_list_of_buttons[i]->y);
		input_window.draw(rectangle);

		//draw the button text
		text.setString(in_list_of_buttons[i]->text);
		text.setPosition(  in_list_of_buttons[i]->x, in_list_of_buttons[i]->y);
		input_window.draw(text);
	}
}

//draw the new canvas when resizing
void draw_new_canvas_size(sf::RenderWindow& input_window, canvas input_canvas, sf::Vector2i increment_decrement_vector, int disp_x, int disp_y, int spacing_x, int spacing_y)
{
	sf::RectangleShape rectangle(sf::Vector2f(spacing_x, spacing_y));
	rectangle.setFillColor(sf::Color(25, 25, 25, 50));
	rectangle.setOutlineThickness(1);
	rectangle.setOutlineColor(sf::Color(30, 30, 30));

	for (int i = 0; i < input_canvas.size_x + increment_decrement_vector.x; i++)
	{
		for (int j = 0; j < input_canvas.size_y + increment_decrement_vector.y; j++)
		{
			rectangle.setPosition(disp_x + i * spacing_x, disp_y + j * spacing_y);
			input_window.draw(rectangle);
		}
	}
}

//returns the index of the button that has been clicked in the gui
int click_inside_index(sf::Vector2i inpt, std::vector<button*> in_list_of_buttons)
{
	for (int i = 0; i < in_list_of_buttons.size(); i++)
	{
		if (in_list_of_buttons[i]->is_inside(inpt.x, inpt.y)) { return i; }
	}
	return -1;
}

//returns true if coordinate is inside of the rect described
bool inside_rect(sf::Vector2i inpt_vector, int pos_x, int pos_y, int width, int height) 
{
	if (inpt_vector.x > pos_x && inpt_vector.x < pos_x + width) 
	{
		if (inpt_vector.y > pos_y && inpt_vector.y < pos_y + height)
		{
			return true;
		}
	}
	return false;
}

//returns a vector which contains the location in which the 
sf::Vector2i cell_location(sf::Vector2i inpt, canvas input_canvas, int spacing_x, int spacing_y)
{

	sf::Vector2i to_return;

	int offset_x;
	offset_x     = inpt.x % spacing_x;
	offset_x     = inpt.x - offset_x;
	offset_x     = offset_x / spacing_x;

	int offset_y;
	offset_y     = inpt.y % spacing_y;
	offset_y     = inpt.y - offset_y;
	offset_y     = offset_y / spacing_y;

	to_return.x = offset_x;
	to_return.y = offset_y;

	return to_return;
}

//main function
int main()
{
	using namespace std;

	//view variables
	int initial_size_x = 1500;
	int initial_size_y = 900;
	sf::View view1(sf::FloatRect(0, 0, initial_size_x, initial_size_y));
	sf::RenderWindow window(sf::VideoMode(1500, 900), "ASIIC editor");
	window.setVerticalSyncEnabled(false);
	window.setView(view1);
	int displacement_x = 350;
	int displacement_y = 50;
	int cell_size_x      = 17;
	int cell_size_y      = 40;
	float zoom = 1.0;

	//tools variables
	int  selecion_mode   = 2;
	bool selection_value = true;
	sf::Vector2i square_selection_initial_point;
	sf::Vector2i square_selection_end_point;

	//the canvas itself
	canvas new_canvas = canvas(40, 15);
	//new_canvas.activ_cells[3][4] = true;
	//new_canvas.cell_letters[3][4] = 'e';
	//new_canvas.cell_letters[3][5] = 'e';
	//new_canvas.cell_letters[3][6] = 'e';

	//variables for the loop
	bool out_of_canvas = true;
	bool left_mouse_button_is_down;
	bool prev_left_mouse_button_is_down = false;
	bool prev_any_key_pressed;
	bool left_mouse_button_just_down;
	bool left_mouse_button_just_up;
	bool prev_any_key_is_pressed;
	sf::Vector2f mouse_position;
	sf::Vector2f prev_mouse_position;
	sf::Vector2f initial_mouse_position;
	sf::Vector2i cell_location_vector;
	sf::Vector2i displacement_v;
	sf::Vector2i selection_position;
	sf::Vector2i increment_decrement_vector;

	//text item for the GUI system
	sf::Font font;
	if (!font.loadFromFile("consolas.ttf")) { /* error... */ }
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setColor(sf::Color::White);

	//button system below
	int  canvas_button_pos_x = 0;
	int  canvas_button_pos_y = 0;
	bool moving_canvas_button = false;
	button canvas_button = button(canvas_button_pos_x, canvas_button_pos_y, 40, 40, "");

	vector<button*> list_of_buttons;
	list_of_buttons.push_back( new button(10,10, 290,50,"save to txt") );
	list_of_buttons.push_back( new button(10,150,290,50,"pencil selection") );
	list_of_buttons.push_back( new button(10,220,290,50,"square selection") );
	list_of_buttons.push_back( new button(10,290,290,50,"Wand selection")   ); // 3
	list_of_buttons.push_back( &canvas_button ); // 4

    //the main loop of the display system. Yet more optimization is needed with the cpu usage...
	sf::Clock clock;while (window.isOpen())
	
	{

		std::cout << "0\n";

		clock.restart();
		
		mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

		out_of_canvas  = !inside_rect((sf::Vector2i)mouse_position, displacement_x, displacement_y, new_canvas.size_x*cell_size_x, new_canvas.size_y*cell_size_y);

		//displacement_v should be a 2d vector in first place
		displacement_v = sf::Vector2i(displacement_x, displacement_y);
		cell_location_vector = cell_location((sf::Vector2i)mouse_position - displacement_v, new_canvas, cell_size_x, cell_size_y);

		left_mouse_button_is_down = sf::Mouse::isButtonPressed(sf::Mouse::Left);

		left_mouse_button_just_down = false;
		if (left_mouse_button_is_down && !prev_left_mouse_button_is_down) { 
			left_mouse_button_just_down = true; //std::cout << "left m";
		}
		left_mouse_button_just_up = false;
		if (!left_mouse_button_is_down && prev_left_mouse_button_is_down) { 
			left_mouse_button_just_up = true; //std::cout << "up";
		}

		canvas_button_pos_x = displacement_x + new_canvas.size_x * cell_size_x + 10;
		canvas_button_pos_y = displacement_y + new_canvas.size_y * cell_size_y + 10;

		std::cout << "1\n";

		//we move the corner button
		if (moving_canvas_button)
		{
			canvas_button.x     += (- prev_mouse_position + mouse_position).x;
			//canvas_button_pos_x += (- prev_mouse_position + mouse_position).x;
			canvas_button.y     += (- prev_mouse_position + mouse_position).y;
			//canvas_button_pos_y += (- prev_mouse_position + mouse_position).y;

			//increment_decrement_vector = cell_location((sf::Vector2i)mouse_position - displacement_v - (sf::Vector2i)initial_mouse_position, new_canvas, cell_size_x, cell_size_y);
			increment_decrement_vector =                               cell_location((sf::Vector2i)mouse_position         - displacement_v , new_canvas, cell_size_x, cell_size_y);
			increment_decrement_vector = increment_decrement_vector - (cell_location((sf::Vector2i)initial_mouse_position - displacement_v , new_canvas, cell_size_x, cell_size_y));

			cout << increment_decrement_vector.x << " O " << increment_decrement_vector.y << "\n";
		}
		else
		{
			canvas_button.x     = canvas_button_pos_x;
			canvas_button.y     = canvas_button_pos_y;
		}

		std::cout << "2\n";

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::MouseWheelMoved)
			{
				zoom += (event.mouseWheel.delta * 0.1);
				//cout << zoom << "\n";
				view1.setSize(initial_size_x * zoom, initial_size_y * zoom);
			}
		}

		std::cout << "3\n";

		//button handling
		if (left_mouse_button_is_down) // && !prev_left_mouse_button_is_down)
		{

			//click inside of a button
			int index = click_inside_index((sf::Vector2i)mouse_position, list_of_buttons);
			if (index != -1) 
			{
				//cout << click_inside_index((sf::Vector2i)mouse_position, list_of_buttons);
				if (index == 0) { new_canvas.save_to("fileee.txt"); }
				if (index == 1) { selecion_mode = 0; }
				if (index == 2) { selecion_mode = 1; }
				if (index == 3) { selecion_mode = 2; }

				if (index == 4 && left_mouse_button_just_down)
				{
					initial_mouse_position = mouse_position;
					moving_canvas_button = true;
				}
			}

			//click happened inside of the canvas
			else if (!out_of_canvas)
			{

				//selection mode, if adding true values or false values...
				if (left_mouse_button_just_down)
				{
					//another comment...
					selection_value = !new_canvas.activ_cells[cell_location_vector.y][cell_location_vector.x];
				}

				//pencil toggle selection
				if (selecion_mode == 0)
				{
					//stuff
					new_canvas.activ_cells[cell_location_vector.y][cell_location_vector.x] = selection_value;
				}

				//square toggle selection mode
				if (selecion_mode == 1)
				{
					if (left_mouse_button_just_down)
					{
						square_selection_initial_point = cell_location_vector;
						square_selection_end_point     = cell_location_vector;
					}
					square_selection_end_point = cell_location_vector;
					new_canvas.set_square_selection_temporal(square_selection_initial_point, square_selection_end_point, selection_value);
				}

				//wand selection mode
				if (selecion_mode == 2 && left_mouse_button_just_down)
				{
					new_canvas.deselect_all();
					new_canvas.select_bucket(cell_location_vector);
				}
			}
		}
		if (left_mouse_button_just_up) 
		{	
			if (moving_canvas_button)
			{
				moving_canvas_button = false;
				new_canvas.resize(increment_decrement_vector.x, increment_decrement_vector.y);
			}
			if (selecion_mode == 0)
			{

			}
			if (selecion_mode == 1)
			{
				new_canvas.overlay_short_matrix(new_canvas.tmp_activ_cells);
				new_canvas.clear_short_matrix();
			}
			if (selecion_mode == 2)
			{

			}
		}

		std::cout << "4\n";

		//text input management, pretty much it works when something in the canvas is selected...
		if ((event.type == sf::Event::TextEntered) && !prev_any_key_pressed) 
		{
			if (event.text.unicode < 256)
			{
				//in case we have more than just one cell selected
				if (new_canvas.return_ammount_selected() > 1)
				{
					char value_to_add = ' ';
					if(event.text.unicode != '\b') { value_to_add = event.text.unicode; }
					new_canvas.set_char_selected(value_to_add);
					new_canvas.deselect_all();
				}

				//in case we have just one cell selected
				else if (new_canvas.return_ammount_selected() == 1)
				{
					char value_to_add = ' ';
					if(event.text.unicode != '\b')
					{ 
						value_to_add = event.text.unicode;

						new_canvas.set_char_selected(value_to_add);
						selection_position = new_canvas.first_position_selection();
						new_canvas.deselect_all();
						new_canvas.activ_cells[selection_position.y][selection_position.x + 1] = true;
					}
					else
					{
						//value_to_add = event.text.unicode;

						new_canvas.set_char_selected(value_to_add);
						selection_position = new_canvas.first_position_selection();
						new_canvas.deselect_all();
						new_canvas.activ_cells[selection_position.y][selection_position.x - 1] = true;
					}
				}
			}
		}

		std::cout << "5\n";

		if (event.type == sf::Event::KeyPressed && !prev_any_key_is_pressed)
		{
			if (new_canvas.return_ammount_selected() == 1)
			{
				selection_position = new_canvas.first_position_selection();

				if (sf::Keyboard::isKeyPressed( sf::Keyboard::Left ))
				{
					if (selection_position.x > 0) 
					{ 
						new_canvas.deselect_all();
						new_canvas.activ_cells[selection_position.y][selection_position.x - 1] = true;
					}
				}
				if (sf::Keyboard::isKeyPressed( sf::Keyboard::Right ))
				{
					if (selection_position.x < new_canvas.size_x - 1) 
					{ 
						new_canvas.deselect_all();
						new_canvas.activ_cells[selection_position.y][selection_position.x + 1] = true;
					}
				}
				if (sf::Keyboard::isKeyPressed( sf::Keyboard::Up ))
				{
					if (selection_position.y > 0) 
					{ 
						new_canvas.deselect_all();
						new_canvas.activ_cells[selection_position.y - 1][selection_position.x] = true;
					}
				}
				if (sf::Keyboard::isKeyPressed( sf::Keyboard::Down ))
				{
					if (selection_position.y < new_canvas.size_y - 1) 
					{ 
						new_canvas.deselect_all();
						new_canvas.activ_cells[selection_position.y + 1][selection_position.x] = true;
					}
				}
			}
		}
		//system to drag the canvas
		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
		{
			displacement_x += mouse_position.x - prev_mouse_position.x;
			displacement_y += mouse_position.y - prev_mouse_position.y;
		}

		std::cout << "6\n";

		//used to clear the selection
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			//this deletes stuff
			new_canvas.deselect_all();			
		}

		/*
		std::cout << "the escape key was pressed" << std::endl;
		std::cout << "control:" << event.key.control << std::endl;
		std::cout << "alt:" << event.key.alt << std::endl;
		std::cout << "shift:" << event.key.shift << std::endl;
		std::cout << "system:" << event.key.system << std::endl;ç
		*/

		std::cout << "7\n";

		//the drawing part of the loop
		window.clear();
		window.draw(text);
			std::cout << "7.0\n";
			draw_grid(      window, new_canvas, displacement_x, displacement_y, cell_size_x, cell_size_y);
			std::cout << "7.1\n";
			draw_selected(  window, new_canvas, displacement_x, displacement_y, cell_size_x, cell_size_y);
			std::cout << "7.2\n";
			draw_characters(window, new_canvas, displacement_x + 10, displacement_y, cell_size_x, cell_size_y, text);
			std::cout << "7.3\n";
			draw_buttons(   window, new_canvas, list_of_buttons, font);
			std::cout << "7.4\n";

			if (moving_canvas_button) draw_new_canvas_size(window, new_canvas, increment_decrement_vector, displacement_x, displacement_y, cell_size_x, cell_size_y);

		std::cout << "8\n";

		window.setView(view1);
		window.display();

		//prev variables
		prev_mouse_position            = mouse_position;
		prev_left_mouse_button_is_down = left_mouse_button_is_down;
		prev_any_key_pressed           = (event.type == sf::Event::TextEntered);
		prev_any_key_is_pressed        = (event.type == sf::Event::KeyPressed);

		//adjusting the timing in the loop
		sf::Time elapsed = clock.getElapsedTime();
		float sleepTime = 1.f / 60.f - elapsed.asSeconds();
		if(sleepTime > 0.f)
		{
		    sf::sleep(sf::seconds(sleepTime));
		}

		std::cout << "9\n";
	}
	return 0;
}


