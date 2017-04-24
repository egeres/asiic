#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "canvas.h"
#include "button.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#ifdef _WIN32 // note the underscore: without it, it's not msdn official!
// Windows (x64 and x86)
#include <windows.h>

#elif __unix__ // all unices, not all compilers
// Unix

#elif __linux__
// linux

#elif __APPLE__
// Mac OS, not sure if this is covered by __posix__ and/or __unix__ though...

#endif

//std::string select_file(canvas& input_canvas)
std::string select_file(bool filehastoexist)
{
	char filename[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a text file to open";
	//ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (filehastoexist)
	{
		ofn.Flags = OFN_FILEMUSTEXIST;
	}

	if (GetOpenFileNameA(&ofn))
	{
		std::cout << "Selected file... \"" << filename << "\"\n";
		return filename;
		//input_canvas.load_text_file(filename);
	}
	else
	{
		// All this stuff below is to tell you exactly how you messed up above.
		// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
		case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
		case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
		case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
		case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
		case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
		case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
		case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
		case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
		case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
		case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
		case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
		case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
		case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
		case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
		default: std::cout << "You cancelled.\n";
		}
	}
}
/*  TO DO

	optimize the cpu usage... maybe is an issue with the joystick ?

	clean the code of the 1.1.0
	clean the code of the 1.2.0

	add debug mode
	find cool use for the enter key

	make center_canvas_in_window() great again, lol

	functionality / tools :
		upper tabs select different canvas's
		canvas are placed in a dynamic vector now
		shorcuts :P
		square selection moving when out of boundaries
		select lines ?
		add ctrl+z
		move selection with middle mouse ?
		blinking cursor displace text sideways both left and right ?
		move selection with arrow keys
		move selection with ctrl+middlemouse

	menu / interface :
		display square selection size
		help window ?
		blinking cursor
		invert colors ?
		right click pie menu ?
		options menu
		startup dialog

	bugs / improvements:
		everything crashes when two file types are the same i sepparated tabs... -_-"
		fix canvas resizing causing issues with te interface
		fix camera zoom
		fix square selection lagging
		fix weird displacement with the canvas drag and drop button
		fix "out of matrix" tier issues with the "blinking cursor"
		fix square selection bugging when leaving things selected
		better text input, keys lag by some reason

*/


/* IMPLENTED / FIXED
	upper tabs for multiple canvas selecion
	spacer class
	GUI object class
	open file menu / load file
	show the "pixel" position
	text_over_toolbar
	tool bar
	button system
	icon for the project
	resize canvas dynamically
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




namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

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

	for (int i = 0; i < input_canvas.size_y; i++)
	{
		for (int j = 0; j < input_canvas.size_x; j++)
		{

	//for (int i = 0; i < input_canvas.activ_cells.size(); i++)
	//{
	//	for (int j = 0; j < input_canvas.activ_cells[0].size(); j++)
	//	{
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

	for (int i = 0; i < input_canvas.size_y; i++)
	{
		for (int j = 0; j < input_canvas.size_x; j++)
		{

	//for (int i = 0; i < input_canvas.cell_letters.size(); i++)
	//{
	//	for (int j = 0; j < input_canvas.cell_letters[0].size(); j++)
	//	{
			text_obj.setPosition(disp_x + j * spacing_x - 10, disp_y + i * spacing_y);
			text_obj.setString(input_canvas.cell_letters[i][j]);
			input_window.draw(text_obj);
		}
	}
}

//function to draw the GUI buttons
/*
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
*/

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
/*
int click_inside_index(sf::Vector2i inpt, std::vector<button*> in_list_of_buttons)
{
	for (int i = 0; i < in_list_of_buttons.size(); i++)
	{
		if (in_list_of_buttons[i]->is_inside(inpt.x, inpt.y)) { return i; }
	}
	return -1;
}
*/

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

//
void draw_text_over_toobox_up(sf::RenderWindow& input_window, std::string input_string,  sf::Text& text_obj, sf::Font fnt)
{

	text_obj.setString(input_string);
	sf::FloatRect bounds = text_obj.getLocalBounds();

	text_obj.setPosition(
		((int)input_window.getSize().x / 2) - bounds.width/2,
		((int)input_window.getSize().y - (int)input_window.getSize().y * 0.15) - 65 + 3
	);

	sf::RectangleShape rectangle(sf::Vector2f(bounds.width + 16, fnt.getLineSpacing(40)));
	rectangle.setFillColor(sf::Color(14, 14, 14));
	rectangle.setPosition(
		((int)input_window.getSize().x / 2) - bounds.width/2 - 8,
		((int)input_window.getSize().y - (int)input_window.getSize().y * 0.15) - 50 + 3
	);

	input_window.draw(rectangle);
	input_window.draw(text_obj);
}

//
void draw_text_over_toobox_bottom(sf::RenderWindow& input_window, std::string input_string,  sf::Text& text_obj, sf::Font fnt)
{

	text_obj.setString(input_string);
	sf::FloatRect bounds = text_obj.getLocalBounds();

	text_obj.setPosition(
		((int)input_window.getSize().x / 2) - bounds.width/2,
		((int)input_window.getSize().y - (int)input_window.getSize().y * 0.15) - 65 + 126
	);

	sf::RectangleShape rectangle(sf::Vector2f(bounds.width + 16, fnt.getLineSpacing(40)));
	rectangle.setFillColor(sf::Color(14, 14, 14));
	rectangle.setPosition(
		((int)input_window.getSize().x / 2) - bounds.width/2 - 8,
		((int)input_window.getSize().y - (int)input_window.getSize().y * 0.15) - 50 + 126
	);

	input_window.draw(rectangle);
	input_window.draw(text_obj);
}

//main function
int main()
{
	using namespace std;

	//view variables
	int initial_size_x = 1500;
	int initial_size_y = 900;
	sf::View view1(sf::FloatRect(0, 0, initial_size_x, initial_size_y));
	sf::RenderWindow window(sf::VideoMode(1500, 900), "ASIIC editor v1.1.0");

	//sf::View view1(55 sf::FloatRect(0, 200, window.GetSize().x, window.GetSize().y);

	window.setVerticalSyncEnabled(true);
	window.setView(view1);
	int displacement_x = 350;
	int displacement_y = 50;
	int cell_size_x      = 17;
	int cell_size_y      = 40;
	float zoom = 1.0;
	std::string string_cell_upper_toolbox = "";

	//tools variables
	int  selection_mode   = 2;
	bool selection_value = true;
	sf::Vector2i square_selection_initial_point;
	sf::Vector2i square_selection_end_point;

	//the canvas itself
	vector<canvas*> canvases;

	canvases.push_back(new canvas(40, 15, "unknow"));
	canvases.push_back(new canvas(80, 15, "unknow"));
	canvases.push_back(new canvas(90, 15, "unknow"));
	canvases.push_back(new canvas(120, 15, "unknow"));

	//canvas new_canvas = canvas(40, 15);
	int active_canvas_index = 0;

	canvas new_canvas = *canvases[active_canvas_index];

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
	sf::Font font_consolas;
	if (!font_consolas.loadFromFile("assets/consolas.ttf")) { /* error... */ }
	sf::Text text_consolas;
	text_consolas.setFont(font_consolas);
	text_consolas.setCharacterSize(30);
	text_consolas.setColor(sf::Color::White);

	sf::Font font_pixel;
	if (!font_pixel.loadFromFile("assets/pixel.ttf")) { /* error... */ }
	sf::Text text_pixel;
	text_pixel.setFont(font_pixel);
	text_pixel.setCharacterSize(40);
	text_pixel.setColor(sf::Color::White);

	//textures
	sf::Texture tex_icon_pencil_selection;
	sf::Texture tex_icon_equal_character_selection;
	sf::Texture tex_icon_resize;
	sf::Texture tex_icon_save;
	sf::Texture tex_icon_square_selection;
	sf::Texture tex_icon_wand_selection;
	sf::Texture tex_icon_folder;
	sf::Texture tex_icon_options;
	sf::Texture tex_icon_resize_b;
	sf::Texture tex_icon_spacing;

	if (!tex_icon_pencil_selection.loadFromFile("assets/icon_pencil_selection.png")) {}
	if (!tex_icon_equal_character_selection.loadFromFile("assets/icon_equal_character_selection.png")) {}
	if (!tex_icon_resize.loadFromFile("assets/icon_resize.png")) {}
	if (!tex_icon_save.loadFromFile("assets/icon_save.png")) {}
	if (!tex_icon_square_selection.loadFromFile("assets/icon_square_selection.png")) {}
	if (!tex_icon_wand_selection.loadFromFile("assets/icon_wand_selection.png")) {}
	if (!tex_icon_folder.loadFromFile("assets/icon_folder.png")) {}
	if (!tex_icon_options.loadFromFile("assets/icon_options.png")) {}
	if (!tex_icon_resize_b.loadFromFile("assets/icon_resize_b.png")) {}
	if (!tex_icon_spacing.loadFromFile("assets/icon_alpha_20_60.png")) {}

	//images & sprites
	sf::Sprite spr_icon_pencil_selection;
	sf::Sprite spr_icon_equal_character_selection;
	sf::Sprite spr_icon_resize;
	sf::Sprite spr_icon_save;
	sf::Sprite spr_icon_square_selection;
	sf::Sprite spr_icon_wand_selection;
	sf::Sprite spr_icon_folder;
	sf::Sprite spr_icon_options;
	sf::Sprite spr_icon_resize_b;
	sf::Sprite spr_icon_spacing;

	spr_icon_pencil_selection.setTexture(tex_icon_pencil_selection);
	spr_icon_equal_character_selection.setTexture(tex_icon_equal_character_selection);
	spr_icon_resize.setTexture(tex_icon_resize);
	spr_icon_save.setTexture(tex_icon_save);
	spr_icon_square_selection.setTexture(tex_icon_square_selection);
	spr_icon_wand_selection.setTexture(tex_icon_wand_selection);
	spr_icon_folder.setTexture(tex_icon_folder);
	spr_icon_options.setTexture(tex_icon_options);
	spr_icon_resize_b.setTexture(tex_icon_resize_b);
	spr_icon_spacing.setTexture(tex_icon_spacing);

	//sound (yeah, there's sound in this software...)
	sf::SoundBuffer buffer_minimal_click;
	if(!buffer_minimal_click.loadFromFile("assets/minimal_clickb.wav")) {}

	sf::Sound sound_minimal_click;
	sound_minimal_click.setBuffer(buffer_minimal_click);
	sound_minimal_click.setVolume(15);

	//upper navigation bar
	navigation_bar_txt upper_toolbar(sf::Vector2i((int)window.getSize().x / 2, (int)window.getSize().y * 0.02), sf::Color(13, 13, 13), 8, 8, "centered", "horizontal", sound_minimal_click);
	//upper_toolbar.list_of_buttons.push_back( new button_image(sf::Vector2i(1, 1),spr_icon_pencil_selection,         "pencil_mode")    );
	upper_toolbar.list_of_buttons.push_back( new button_text(sf::Vector2i(1, 1), "unknow", "pressed_6on_1", font_pixel, sf::Color(25, 25, 25), 5) );
	upper_toolbar.list_of_buttons.push_back( new button_text(sf::Vector2i(1, 1), "file_text_0.txt", "pr6essed_button_1", font_pixel, sf::Color(25, 25, 25), 5));
	upper_toolbar.list_of_buttons.push_back( new button_text(sf::Vector2i(1, 1), "file_text_1.txt", "press3ed_button_1", font_pixel, sf::Color(25, 25, 25), 5));
	upper_toolbar.list_of_buttons.push_back( new button_text(sf::Vector2i(1, 1), "file_text_2.txt", "presse3d_button_1", font_pixel, sf::Color(25, 25, 25), 5) );

	upper_toolbar.update();

	//navigation bar
	navigation_bar_img main_toolbar(sf::Vector2i((int)window.getSize().x / 2, (int)window.getSize().y - (int)window.getSize().y * 0.15), sf::Color(13, 13, 13), 5, 5, "centered", "horizontal", sound_minimal_click);
	main_toolbar.list_of_buttons.push_back( new button_image(sf::Vector2i(1, 1),spr_icon_pencil_selection,         "pencil_mode")    );
	main_toolbar.list_of_buttons.push_back( new button_image(sf::Vector2i(1, 1),spr_icon_square_selection,         "square_mode")    );
	main_toolbar.list_of_buttons.push_back( new button_image(sf::Vector2i(1, 1),spr_icon_wand_selection,           "wand_mode")      );
	main_toolbar.list_of_buttons.push_back( new button_image(sf::Vector2i(1, 1),spr_icon_equal_character_selection, "similarity_mode"));

	main_toolbar.list_of_buttons.push_back( new button_image(sf::Vector2i(1, 1),spr_icon_spacing,""));
	//main_toolbar.list_of_buttons.push_back( new spacer(sf::Vector2i(15, 1)));

	main_toolbar.list_of_buttons.push_back( new button_image(sf::Vector2i(1, 1),spr_icon_save,                     "save")           );
	main_toolbar.list_of_buttons.push_back( new button_image(sf::Vector2i(1, 1),spr_icon_folder,                   "open_file")      );
	main_toolbar.list_of_buttons.push_back( new button_image(sf::Vector2i(1, 1),spr_icon_options,                  "options")        );
	main_toolbar.update();

	//button system below
	int  canvas_button_pos_x = 0;
	int  canvas_button_pos_y = 0;
	bool moving_canvas_button = false;
	button canvas_button = button(canvas_button_pos_x, canvas_button_pos_y, 40, 40, "");

	button_image canvas_button_image = button_image(sf::Vector2i(canvas_button_pos_x, canvas_button_pos_y), spr_icon_resize_b, "resize");

	/*
	vector<button*> list_of_buttons;
	list_of_buttons.push_back( new button(10,10, 290,50,"save to txt")         );
	list_of_buttons.push_back( new button(10,150,290,50,"pencil selection")    );
	list_of_buttons.push_back( new button(10,220,290,50,"square selection")    );
	list_of_buttons.push_back( new button(10,290,290,50,"wand selection")      ); // 3
	list_of_buttons.push_back( new button(10,360,290,50,"character selection") ); // 4
	list_of_buttons.push_back( &canvas_button ); // 5
	*/

    //the main loop of the display system. Yet more optimization is needed with the cpu usage...
	sf::Clock clock;while (window.isOpen())

	{

		//std::cout << "0\n";

		clock.restart();

		view1.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
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

		canvas_button_pos_x = displacement_x + new_canvas.size_x * cell_size_x + 6;
		canvas_button_pos_y = displacement_y + new_canvas.size_y * cell_size_y + 6;

		//std::cout << "1\n";

		//we move the corner button
		if (moving_canvas_button)
		{
			//canvas_button.x     += (- prev_mouse_position + mouse_position).x;
			canvas_button_pos_x += (- prev_mouse_position + mouse_position).x;
			canvas_button_image.pos.x += (- prev_mouse_position + mouse_position).x;

			//canvas_button.y     += (- prev_mouse_position + mouse_position).y;
			canvas_button_pos_y += (- prev_mouse_position + mouse_position).y;
			canvas_button_image.pos.y += (- prev_mouse_position + mouse_position).y;

			canvas_button_image.spr.setPosition(canvas_button_image.pos.x, canvas_button_image.pos.y);

			//increment_decrement_vector = cell_location((sf::Vector2i)mouse_position - displacement_v - (sf::Vector2i)initial_mouse_position, new_canvas, cell_size_x, cell_size_y);
			increment_decrement_vector =                               cell_location((sf::Vector2i)mouse_position         - displacement_v , new_canvas, cell_size_x, cell_size_y);
			increment_decrement_vector = increment_decrement_vector - (cell_location((sf::Vector2i)initial_mouse_position - displacement_v , new_canvas, cell_size_x, cell_size_y));

			//cout << increment_decrement_vector.x << " O " << increment_decrement_vector.y << "\n";
		}
		else
		{
			//canvas_button.x     = canvas_button_pos_x;
			canvas_button_image.pos.x = canvas_button_pos_x;

			//canvas_button.y     = canvas_button_pos_y;
			canvas_button_image.pos.y = canvas_button_pos_y;

			//canvas_button_image.spr.setPosition(canvas_button_pos_x, canvas_button_pos_y);
			canvas_button_image.spr.setPosition(canvas_button_image.pos.x, canvas_button_image.pos.y);

		}

		//std::cout << "2\n";

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

		//std::cout << "3\n";

		//button handling
		if (left_mouse_button_is_down) // && !prev_left_mouse_button_is_down)
		{

			//std::cout << "3.0\n";

			string index = main_toolbar.check_click((sf::Vector2i)mouse_position);
			int index_esto = upper_toolbar.index_by_click((sf::Vector2i)mouse_position);

			if (!index.empty())
			{
				if (index == "pencil_mode")     { selection_mode = 0; }
				if (index == "square_mode")     { selection_mode = 1; }
				if (index == "wand_mode")       { selection_mode = 2; }
				if (index == "similarity_mode") { selection_mode = 3; }

				if (index == "save")
				{ 
					if (new_canvas.file_route.empty())
					{
						new_canvas.file_route = select_file(false);
					}

					//new_canvas.save_to("lastest_canvas_save.txt");
					new_canvas.save_to(new_canvas.file_route);
				}

				if (index == "open_file")
				{
					//std::string select_file(new_canvas);
					std::string to_loaaaad = select_file(true);
					//input_canvas.load_text_file(to_loaaaad);
					new_canvas.load_text_file(to_loaaaad);

					canvases[active_canvas_index]->file_route  = new_canvas.file_route;
					canvases[active_canvas_index]->canvas_name = new_canvas.canvas_name;
				}
			}

			else if (index_esto != -1 && left_mouse_button_just_down)
			{
				//std::vector<canvas>& referencia = *canvases;
				//*canvases[active_canvas_index].cell_letters = &new_canvas.cell_letters;
				//(canvases->at(active_canvas_index)).cell_letters = new_canvas.cell_letters;

				std::cout << "empezando \n";

				canvases[active_canvas_index]->activ_cells.clear();
				canvases[active_canvas_index]->tmp_activ_cells.clear();
				canvases[active_canvas_index]->cell_letters.clear();

				canvases[active_canvas_index]->activ_cells = new_canvas.activ_cells;
				canvases[active_canvas_index]->tmp_activ_cells = new_canvas.tmp_activ_cells;
				canvases[active_canvas_index]->cell_letters = new_canvas.cell_letters;
				canvases[active_canvas_index]->size_x = new_canvas.size_x;
				canvases[active_canvas_index]->size_y = new_canvas.size_y;

				active_canvas_index = index_esto;

				new_canvas = *canvases[active_canvas_index];

				std::cout << "finisheado \n";

			}

			//string index = canvas_button_image.is_inside((sf::Vector2i)mouse_position);

			else if (canvas_button_image.is_inside((sf::Vector2i)mouse_position) && left_mouse_button_just_down)
			{
				initial_mouse_position = mouse_position;
				moving_canvas_button = true;
				sound_minimal_click.play();
			}

			//std::cout << "3.0.0\n";

			/*
			//click inside of a button
			int index = click_inside_index((sf::Vector2i)mouse_position, list_of_buttons);
			if (index != -1)
			{
				//cout << click_inside_index((sf::Vector2i)mouse_position, list_of_buttons);
				if (index == 0) { new_canvas.save_to("lastest_canvas_save.txt"); }
				if (index == 1) { selection_mode = 0; }
				if (index == 2) { selection_mode = 1; }
				if (index == 3) { selection_mode = 2; }
				if (index == 4) { selection_mode = 3; }

				if (index == 5 && left_mouse_button_just_down)
				{
					initial_mouse_position = mouse_position;
					moving_canvas_button = true;
				}
			}
			*/

			//click happened inside of the canvas
			else if (!out_of_canvas)
			{

				//std::cout << "3.0.2\n";

				//selection mode, if adding true values or false values...
				if (left_mouse_button_just_down)
				{
					//another comment...
					//std::cout << "I clicked in : [" << cell_location_vector.x << " " << cell_location_vector.y << "] ";
					//std::cout << " The size is : [" << new_canvas.activ_cells[0].size() << " " << new_canvas.activ_cells.size() << "]";
					//std::cout << " size_x,size_y is : [" << new_canvas.size_x << " " << new_canvas.size_y << "]\n";
					selection_value = !new_canvas.activ_cells[cell_location_vector.y][cell_location_vector.x];
				}

				//std::cout << "3.0.3\n";

				//pencil toggle selection
				if (selection_mode == 0)
				{
					//stuff
					new_canvas.activ_cells[cell_location_vector.y][cell_location_vector.x] = selection_value;
				}

				//square toggle selection mode
				if (selection_mode == 1)
				{
					//std::cout << "3.0.4\n";

					if (left_mouse_button_just_down)
					{
						square_selection_initial_point = cell_location_vector;
						square_selection_end_point     = cell_location_vector;
					}
					square_selection_end_point = cell_location_vector;
					new_canvas.set_square_selection_temporal(square_selection_initial_point, square_selection_end_point, selection_value);
				}

				//std::cout << "3.0.5\n";

				//wand selection mode
				if (selection_mode == 2 && left_mouse_button_just_down)
				{
					new_canvas.deselect_all();
					new_canvas.select_bucket(cell_location_vector);
				}

				//wand selection mode
				if (selection_mode == 3 && left_mouse_button_just_down)
				{
					char selecion_character = new_canvas.cell_letters[cell_location_vector.y][cell_location_vector.x];
					new_canvas.deselect_all();
					new_canvas.equal_character_selection(selecion_character);
				}
			}
		}
		if (left_mouse_button_just_up)
		{
			//std::cout << "3.1\n";

			if (moving_canvas_button)
			{
				moving_canvas_button = false;
				new_canvas.resize(increment_decrement_vector.x, increment_decrement_vector.y);
			}
			if (selection_mode == 0)
			{

			}
			if (selection_mode == 1)
			{
				new_canvas.overlay_short_matrix(new_canvas.tmp_activ_cells);
				new_canvas.clear_short_matrix();
			}
			if (selection_mode == 2)
			{

			}
		}

		//std::cout << "4\n";

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

		//std::cout << "5\n";

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

		//std::cout << "6\n";

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

		//std::cout << "7\n";

		//the drawing part of the loop
		window.clear();
		window.draw(text_consolas);
			//std::cout << "7.0\n";
			draw_grid(      window, new_canvas, displacement_x, displacement_y, cell_size_x, cell_size_y);
			//std::cout << "7.1\n";
			draw_selected(  window, new_canvas, displacement_x, displacement_y, cell_size_x, cell_size_y);
			//std::cout << "7.2\n";
			draw_characters(window, new_canvas, displacement_x + 10, displacement_y, cell_size_x, cell_size_y, text_consolas);
			//std::cout << "7.3\n";
			//draw_buttons(   window, new_canvas, list_of_buttons, font);
			//std::cout << "7.4\n";




			window.draw(canvas_button_image.spr);

			if (moving_canvas_button) draw_new_canvas_size(window, new_canvas, increment_decrement_vector, displacement_x, displacement_y, cell_size_x, cell_size_y);

			main_toolbar.render( window, (sf::Vector2i)mouse_position);

			for (int i = 0; i< upper_toolbar.list_of_buttons.size();i++)
			{

				//canvases[i]->canvas_name = "polla";

				upper_toolbar.list_of_buttons[i]->str = canvases[i]->canvas_name;

				//if (canvases[i]->canvas_name != "") {
				if (!canvases[i]->canvas_name.empty())
				{
					//std::cout <<  "|> " << canvases[i]->canvas_name;
					//std::cout << " |> " << canvases[i]->file_route << "\n";
				}
				//upper_toolbar.list_of_buttons[i]->str = "fu";
				//upper_toolbar.list_of_buttons[i]->txt_obj.setString("fu") = "fu";
			}

			upper_toolbar.update();
			upper_toolbar.render(window, (sf::Vector2i)mouse_position);

			if (!out_of_canvas)
			{
				string_cell_upper_toolbox = "";
				string_cell_upper_toolbox += "<";
				string_cell_upper_toolbox += patch::to_string(cell_location_vector.x);
				string_cell_upper_toolbox += " ";
				string_cell_upper_toolbox += patch::to_string(cell_location_vector.y);
				string_cell_upper_toolbox += ">";
			}
			else
			{
				string_cell_upper_toolbox = "< >";
			}

			draw_text_over_toobox_up(window, string_cell_upper_toolbox, text_pixel, font_pixel);

			if (main_toolbar.check_click((sf::Vector2i)mouse_position) != "" && main_toolbar.check_click((sf::Vector2i)mouse_position) != "clicked the toolbar...")
			{
				std::string texti_pixel;
				texti_pixel = main_toolbar.check_click((sf::Vector2i)mouse_position);
				draw_text_over_toobox_bottom(window, texti_pixel, text_pixel, font_pixel);
			}

		//std::cout << "8\n";

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

		//std::cout << "9\n";
	}
	return 0;
}


