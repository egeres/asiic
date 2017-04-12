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
		wand selection
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

	bugs / improvements:
		fix issue with pencil selection crashing w/ just 1 click
		fix square selection lagging
		fix "out of matrix" tier issues with the "blinking cursor"
		better text input, keys lag by some reason

*/


/* IMPLENTED / FIXED
	backspace functionality implemented
	type-in as normal text implemented
	correct mouse in view
	better movement
	save to .txt
	pencil toggle selection
	square selection
	text size corrected
*/

void draw_grid(sf::RenderWindow& input_window, canvas input_canvas, int disp_x, int disp_y, int spacing_x, int spacing_y)
{

	sf::RectangleShape rectangle(sf::Vector2f(spacing_x, spacing_y));
	rectangle.setFillColor(sf::Color(5, 5, 5));
	rectangle.setOutlineThickness(1);
	rectangle.setOutlineColor(sf::Color(10, 10, 10));

	for (int i = 0; i < input_canvas.size_x; i++) {
		for (int j = 0; j < input_canvas.size_y; j++) {

			rectangle.setPosition(disp_x + i * spacing_x, disp_y + j * spacing_y);
			input_window.draw(rectangle);

		}
	}
}

void draw_selected(sf::RenderWindow& input_window, canvas input_canvas, int disp_x, int disp_y, int spacing_x, int spacing_y)
{

	std::cout << "couuuuuuuuut0";
	input_canvas.overlay_short_matrix(input_canvas.tmp_activ_cells);

	std::cout << "couuuuuuuuut1";

	sf::RectangleShape rectangle(sf::Vector2f(spacing_x, spacing_y));
	std::cout << "couuuuuuuuut2";
	rectangle.setFillColor(sf::Color(20, 20, 20));
	//rectangle.setOutlineThickness(1);
	//rectangle.setOutlineColor(sf::Color(100, 100, 100));

	std::cout << "couuuuuuuuut3";
	for (int i = 0; i < input_canvas.size_y; i++) {
		for (int j = 0; j < input_canvas.size_x; j++) {

			if (input_canvas.activ_cells[i][j]) {

				rectangle.setPosition(disp_x+j*spacing_x, disp_y+i*spacing_y);

				input_window.draw(rectangle);
			}

		}
	}
	std::cout << "couuuuuuuuut4";
}

void draw_characters(sf::RenderWindow& input_window, canvas input_canvas, int disp_x, int disp_y, int spacing_x, int spacing_y, sf::Text& text_obj)
{

	sf::RectangleShape rectangle(sf::Vector2f(spacing_x, spacing_y));
	rectangle.setFillColor(sf::Color(20, 20, 20));
	//rectangle.setOutlineThickness(1);
	//rectangle.setOutlineColor(sf::Color(100, 100, 100));

	for (int i = 0; i < input_canvas.size_y; i++) {
		for (int j = 0; j < input_canvas.size_x; j++) {

			text_obj.setPosition(disp_x + j * spacing_x - 10, disp_y + i * spacing_y);

			text_obj.setString(input_canvas.cell_letters[i][j]);
			input_window.draw(text_obj);

		}
	}
}

void draw_buttons(sf::RenderWindow& input_window, canvas input_canvas, std::vector<button*> in_list_of_buttons, sf::Font font)
{

	//sf::RectangleShape rectangle(sf::Vector2f(1, 1));
	//rectangle.setFillColor(      sf::Color(80, 80, 80)         );

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setColor(sf::Color::White);
	
	for (int i = 0; i < in_list_of_buttons.size(); i++)
	{
		sf::RectangleShape rectangle(sf::Vector2f(in_list_of_buttons[i]->width, in_list_of_buttons[i]->height));
		rectangle.setFillColor(      sf::Color(80, 80, 80)         );
		rectangle.setPosition(in_list_of_buttons[i]->x, in_list_of_buttons[i]->y);
		input_window.draw(rectangle);

		text.setString(in_list_of_buttons[i]->text);
		text.setPosition(  in_list_of_buttons[i]->x, in_list_of_buttons[i]->y);
		input_window.draw(text);

		//view1.draw(rectangle);

		//std::cout << in_list_of_buttons[i]->x << in_list_of_buttons[i]->y << in_list_of_buttons[i]->width << in_list_of_buttons[i]->height << "\n";
	}
}

int click_inside_index(sf::Vector2i inpt, std::vector<button*> in_list_of_buttons)
{

	for (int i = 0; i < in_list_of_buttons.size(); i++)
	{
		if (in_list_of_buttons[i]->is_inside(inpt.x, inpt.y)) { return i; }
	}
	return -1;
}

bool inside_rect(int in_x, int in_y, int pos_x, int pos_y, int width, int height) 
{

	if (in_x > pos_x && in_x < pos_x + width) 
	{
		if (in_y > pos_y && in_y < pos_y + height) 
		{
			return true;
		}
	}
	return false;
}

sf::Vector2i cell_location(sf::Vector2i inpt, canvas input_canvas, int spacing_x, int spacing_y) {


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

int main()
{
	using namespace std;

	int initial_size_x = 1500;
	int initial_size_y = 900;

	int  selecion_mode   = 1;
	bool selection_value = true;
	sf::Vector2i square_selection_initial_point;
	sf::Vector2i square_selection_end_point;

	sf::View view1(sf::FloatRect(0, 0, initial_size_x, initial_size_y));

	sf::RenderWindow window(sf::VideoMode(1500, 900), "ASIIC editor");
	window.setVerticalSyncEnabled(false);

	//window.UseVerticalSynch(false);

	window.setView(view1);

	int displacement_x = 350;
	int displacement_y = 50;
	int cell_size_x      = 17;
	int cell_size_y      = 40;

	float zoom = 1.0;

	canvas new_canvas = canvas(60, 18);

	//new_canvas.activ_cells[3][4] = true;
	//new_canvas.cell_letters[3][4] = 'e';
	//new_canvas.cell_letters[3][5] = 'e';
	//new_canvas.cell_letters[3][6] = 'e';

	cout << "yy444y\n";

	bool out_of_canvas = true;

	bool mouse_button_down;
	bool prev_mouse_button_down = false;
	bool left_mouse_button_just_down;
	bool left_mouse_button_just_up;

	sf::Vector2i mouse_position;
	sf::Vector2f prev_mouse_position;
	sf::Vector2i cell_location_vector;

	sf::Vector2i displacement_v;


	sf::Font font;
	if (!font.loadFromFile("consolas.ttf"))
	{
	    // error...
	}

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setColor(sf::Color::White);

	int canvas_button_pos_x = 0;
	int canvas_button_pos_y = 0;
	
	button canvas_button = button(canvas_button_pos_x, canvas_button_pos_y, 40, 40, "");

	vector<button*> list_of_buttons;

	bool prev_any_key_pressed;

	list_of_buttons.push_back( new button(10,10, 290,50,"save to txt") );
	list_of_buttons.push_back( new button(10,150,290,50,"pencil selection") );
	list_of_buttons.push_back( new button(10,220,290,50,"square selection") );
	list_of_buttons.push_back( &canvas_button );
	//list_of_buttons.push_back( new button(10,70,150,50,"awesom") );



	//button new_leaf = button(10,10,150,50,"tananaa");

	/*
	std::cout << list_of_buttons[0]->x;
	std::cout << list_of_buttons[0]->y;
	std::cout << list_of_buttons[0]->width;
	*/

	//std::cout << new_leaf.x;
	//std::cout << new_leaf.y;
	//std::cout << new_leaf.width;
	//std::cout << new_leaf.height;
	//std::cout << new_leaf.text;
	
	//const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);

	//sf::Clock clock;
    //sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Clock clock;
	while (window.isOpen())
	{

		// Inside the main loop
		clock.restart();
		
		// Do the drawing etc.
		
        //sf::Time elapsedTime = clock.restart();
        //timeSinceLastUpdate += elapsedTime;
        //while (timeSinceLastUpdate > TimePerFrame)
        //{
        //    timeSinceLastUpdate -= TimePerFrame;
		//
        //    //processEvents();
        //    update(TimePerFrame);
		//
        //}

		//update variables

		//mouse_position       = sf::Mouse::getPosition(window);
		sf::Vector2f mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		//mouse_position.x = event ;
		//mouse_position.y = ;

		cout << "y444yy||n\n";

		out_of_canvas        = !inside_rect( mouse_position.x, mouse_position.y, displacement_x, displacement_y, new_canvas.size_x*cell_size_x, new_canvas.size_y*cell_size_y);
		displacement_v.x     = displacement_x;
		displacement_v.y     = displacement_y;

		cout << "yy32133244y\n";

		cell_location_vector = cell_location((sf::Vector2i)mouse_position - displacement_v, new_canvas, cell_size_x, cell_size_y);
		
		//cell_location_vector = (sf::Vector2i)mouse_position;

		mouse_button_down    = sf::Mouse::isButtonPressed(sf::Mouse::Left);

		left_mouse_button_just_down = false;
		if (mouse_button_down && !prev_mouse_button_down) { left_mouse_button_just_down = true; std::cout << "left m";}

		left_mouse_button_just_up = false;
		if (!mouse_button_down && prev_mouse_button_down) { left_mouse_button_just_up = true; std::cout << "up";}

		cout << "yy4333rry\n";
		canvas_button_pos_x = displacement_x + new_canvas.size_x * cell_size_x + 10;
		canvas_button_pos_y = displacement_y + new_canvas.size_y * cell_size_y + 10;

		cout << "        5555\n";

		canvas_button.x = canvas_button_pos_x;
		canvas_button.y = canvas_button_pos_y;

		//loop itself
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseWheelMoved)
			{
				zoom += (event.mouseWheel.delta * 0.1);
				
				cout << zoom << "\n";

				//view1.zoom(zoom);
				view1.setSize(initial_size_x * zoom, initial_size_y * zoom);
			}
		}







		cout << "   33444444 5555\n";



		if (mouse_button_down) // && !prev_mouse_button_down)
		{

			//click inside of a button

			int index = click_inside_index((sf::Vector2i)mouse_position, list_of_buttons);

			if (index != -1) 
			{
				//cout << click_inside_index((sf::Vector2i)mouse_position, list_of_buttons);

				if (index == 0) { cout << "eg";new_canvas.save_to("fileee.txt"); }

				if (index == 1) { selecion_mode = 0; }
				if (index == 2) { selecion_mode = 1; }
			}
			//click inside of the canvas
			else if (!out_of_canvas)
			{

				//cout << std::to_string(cell_location_vector.x) << "  "<< std::to_string(cell_location_vector.y) << "  :";

				//selection mode, if adding true vlaues or false values...
				if (left_mouse_button_just_down) { 
					cout << "77_happy_free";
					//selection_value = !new_canvas.activ_cells[cell_location_vector.x][cell_location_vector.y];
					selection_value = !new_canvas.activ_cells[cell_location_vector.y][cell_location_vector.x];
				}

				//pencil toggle selection
				if (selecion_mode == 0)
				{
					cout << "yyy";
					new_canvas.activ_cells[cell_location_vector.x][cell_location_vector.y] = selection_value;
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

					cout << "setsqqee";
					new_canvas.set_square_selection_temporal(square_selection_initial_point, square_selection_end_point, selection_value);

				}
			}
			
		}
		cout << " 88787777775\n";
		if (left_mouse_button_just_up) 
		{	
			if (selecion_mode == 0)
			{

			}
			if (selecion_mode == 1)
			{
				cout << "34";
				new_canvas.overlay_short_matrix(new_canvas.tmp_activ_cells);
				new_canvas.clear_short_matrix();
			}
		}



		cout << "_______________2\n";








		//text input...
		if ((event.type == sf::Event::TextEntered) && !prev_any_key_pressed) {

			if (event.text.unicode < 256)
			{

				if (new_canvas.return_ammount_selected() > 1)
				{
					// cosas
					char value_to_add = ' ';
					if(event.text.unicode != '\b') { value_to_add = event.text.unicode; }

					new_canvas.set_char_selected(value_to_add);
					new_canvas.deselect_all();

				}
				else if (new_canvas.return_ammount_selected() == 1)
				{
					// mas cosas

					char value_to_add = ' ';
					if(event.text.unicode != '\b')
					{ 
						value_to_add = event.text.unicode;

						new_canvas.set_char_selected(value_to_add);
						sf::Vector2i selection_position = new_canvas.first_position_selection();
						new_canvas.deselect_all();
						new_canvas.activ_cells[selection_position.y][selection_position.x + 1] = true;
					}
					else
					{
						//value_to_add = event.text.unicode;

						new_canvas.set_char_selected(value_to_add);
						sf::Vector2i selection_position = new_canvas.first_position_selection();
						new_canvas.deselect_all();
						new_canvas.activ_cells[selection_position.y][selection_position.x - 1] = true;
					}
				}
			}
		}

		cout << "_______________3\n";

		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
		{
			displacement_x += mouse_position.x - prev_mouse_position.x;
			displacement_y += mouse_position.y - prev_mouse_position.y;
		}

		cout << "_______________4\n";

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
				cout << "efg";
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

		cout << "_______________5\n";

		window.draw(text);

		cout << "_______________6\n";

		draw_grid(      window, new_canvas, displacement_x, displacement_y, cell_size_x, cell_size_y);

		cout << "_______________7\n";

		draw_selected(  window, new_canvas, displacement_x, displacement_y, cell_size_x, cell_size_y);

		cout << "_______________8\n";

		draw_characters(window, new_canvas, displacement_x + 10, displacement_y, cell_size_x, cell_size_y, text);

		cout << "_______________9\n";

		draw_buttons(   window, new_canvas, list_of_buttons, font);

		cout << "_______________10\n";
		

		window.setView(view1);
		window.display();

		//prev variables
		prev_mouse_position    = mouse_position;
		prev_mouse_button_down = mouse_button_down;
		prev_any_key_pressed   = (event.type == sf::Event::TextEntered);

		sf::Time elapsed = clock.getElapsedTime();
		float sleepTime = 1.f / 60.f - elapsed.asSeconds();
		if(sleepTime > 0.f)
		{
		    sf::sleep(sf::seconds(sleepTime));
		}
	}

	return 0;
}


