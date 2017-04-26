#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "canvas.h"
#include "button.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#ifdef _WIN32
#include <windows.h>

//funtion used to select a file with the system interface. It should be replaced with GTK or at least make this work in mac/linux
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
		//std::cout << "Selected file... \"" << filename << "\"\n";
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

#elif __unix__
#elif __linux__
#elif __APPLE__
#endif

/*  TO DO

	optimize the cpu usage... maybe is an issue with the joystick ?

	clean the code of the 1.1.0
	clean the code of the 1.2.0

	add debug mode
	find cool use for the enter key

	make center_canvas_in_window() great again, lol

	functionality / tools :
		
		shorcuts :P
		square selection moving when out of boundaries
		select lines ?
		add ctrl+z
		blinking cursor displace text sideways both left and right ?
		move selection with arrow keys

	menu / interface :
		create an add button for creating more tabs
		display square selection size
		help window ?
		blinking cursor
		invert colors option ?
		right click pie menu ?
		options menu
		startup dialog

	bugs / improvements:
		everything crashes if input file is empty
		remove the canvas input_value from the function cell_location()
		size of upper toolbar can sometimes be incorrect ? or.. the buttons ? dunno
		there's a quintillion of warnings about possible data loss...
		fix square selection lagging
		fix weird displacement with the canvas drag and drop button
		fix "out of matrix" tier issues with the "blinking cursor" when moving left
		fix square selection bugging when leaving things selected
		better text input, keys lag by some reason
	
		maybe it's more efficient to use instead of the  current events system ? :

			std::cout << "control:" << event.key.control << std::endl;
			std::cout << "alt:" << event.key.alt << std::endl;
			std::cout << "shift:" << event.key.shift << std::endl;

	planing for future shorcuts :

		ctrl + tab         | change to the tab on the left
		ctrl + tab + shift | change to the tab on the right
		ctrl + w           | close tab
		ctrl + t           | new tab

		ctrl + s           | save
		ctrl + o           | open

		ctrl + n           | pencil mode
		ctrl + r           | rectangle mode
		ctrl + w           | wand mode
		ctrl + e           | equal character mode

		shift + leftmouse  | add selecion, don't erase
		
		ctrl + middlemouse | move selection
*/

/* IMPLENTED / FIXED
	drag and drop system
	'color picker' implemented
	fix canvas resizing causing issues with te interface
	everything crashes when two file types are the same i sepparated tabs... -_-"
	fix camera zoom
	fix "out of matrix" tier issues with the "blinking cursor" when moving right
	fixed issue with the main active canvas
	upper tabs select different canvases
	canvases are located in a dynamic vector now
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


//used in order to convert ints to strings.
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
	rectangle.setOutlineColor(sf::Color(13, 13, 13));
	for (int i = 0; i < input_canvas.size_x; i++)
	{
		for (int j = 0; j < input_canvas.size_y; j++)
		{
			rectangle.setPosition(disp_x + i * spacing_x, disp_y + j * spacing_y);
			input_window.draw(rectangle);
		}
	}
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

//function which draws the selected cells in the grid
void draw_selected(sf::RenderWindow& input_window, canvas input_canvas, int disp_x, int disp_y, int spacing_x, int spacing_y)
{
	//just to crearify, tmp_activ_cells is a temporal matrix created by the square selection
	input_canvas.overlay_short_matrix(input_canvas.tmp_activ_cells);
	sf::RectangleShape rectangle(sf::Vector2f(spacing_x, spacing_y));
	rectangle.setFillColor(sf::Color(20, 20, 20));

	for (int i = 0; i < input_canvas.size_y; i++)
	{
		for (int j = 0; j < input_canvas.size_x; j++)
		{
			if (input_canvas.activ_cells[i][j]) 
			{
				rectangle.setPosition(disp_x+j*spacing_x, disp_y+i*spacing_y);
				input_window.draw(rectangle);
			}

		}
	}
}


//function which draws... things ? I need to sleep more, lol
void draw_drag_and_drop(sf::RenderWindow& input_window, canvas input_canvas, int disp_x, int disp_y, int spacing_x, int spacing_y, sf::Vector2i displacement_from_mouses, canvas new_canvas)
{
	//just to crearify, tmp_activ_cells is a temporal matrix created by the square selection
	//input_canvas.overlay_short_matrix(input_canvas.tmp_activ_cells);
	sf::RectangleShape rectangle(sf::Vector2f(spacing_x, spacing_y));
	rectangle.setFillColor(sf::Color(29, 29, 29));

	sf::Vector2i temporal_disp = cell_location(displacement_from_mouses , new_canvas, spacing_x, spacing_y);
	//std::cout << temporal_disp.x << " _ " << temporal_disp.y << "    " 
	//<< displacement_from_mouses.x << " _ " << displacement_from_mouses.y << "\n";

	for (int i = 0; i < input_canvas.size_y; i++)
	{
		for (int j = 0; j < input_canvas.size_x; j++)
		{
			if (input_canvas.activ_cells[i][j]) 
			{
				rectangle.setPosition((temporal_disp.x*spacing_x) +  disp_x  +  j*spacing_x, (temporal_disp.y*spacing_y) + disp_y + i*spacing_y);
				input_window.draw(rectangle);
			}
		}
	}
}

//function to draw the characters from the matrix
void draw_characters(sf::RenderWindow& input_window, canvas input_canvas, int disp_x, int disp_y, int spacing_x, int spacing_y, sf::Text& text_obj)
{
	sf::RectangleShape rectangle(sf::Vector2f(spacing_x, spacing_y));
	rectangle.setFillColor(sf::Color(20, 20, 20));

	for (int i = 0; i < input_canvas.size_y; i++)
	{
		for (int j = 0; j < input_canvas.size_x; j++)
		{
			text_obj.setPosition(disp_x + j * spacing_x - 10, disp_y + i * spacing_y);
			text_obj.setString(input_canvas.cell_letters[i][j]);
			input_window.draw(text_obj);
		}
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

//draws a text string (with a sexy coloured background) above the toolbox from below
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

//draws a text string (with a sexy coloured background) below the toolbox from below
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

		sf::View hud_view(   sf::FloatRect(0, 0, initial_size_x, initial_size_y));
		sf::View canvas_view(sf::FloatRect(0, 0, initial_size_x, initial_size_y));

		sf::RenderWindow window(sf::VideoMode(1500, 900), "ASIIC editor 0.2.2");
		window.setVerticalSyncEnabled(true);

		int displacement_x = 350;
		int displacement_y = 50;
		int cell_size_x      = 17;
		int cell_size_y      = 40;
		float zoom = 1.0;
		std::string string_cell_upper_toolbox = "";

	//tools variables
		int  selection_mode   = 0;
		bool selection_value = true;
		sf::Vector2i square_selection_initial_point;
		sf::Vector2i square_selection_end_point;
		bool moving_selection_around = false;
		bool prev_moving_selection_around = false;
		sf::Vector2i drag_and_drop_starting_point;
		sf::Vector2i drag_and_drop_cell_displacement;

	//the canvas itself
		vector<canvas*> canvases;
		canvases.push_back(new canvas(40, 15, "unknow"));
		canvases.push_back(new canvas(45, 17, "unknow"));
		int active_canvas_index = 0;
		canvas new_canvas = *canvases[active_canvas_index];

	//variables for the loop
		bool out_of_canvas = true;
		bool left_mouse_button_is_down;
		bool prev_left_mouse_button_is_down = false;
		bool prev_any_key_pressed;
		bool left_mouse_button_just_down;
		bool left_mouse_button_just_up;
		bool prev_any_key_is_pressed;
		sf::Vector2f hud_mouse_position;
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
		sf::Texture oil_on_canvas;

	//we load textures
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
		if (!oil_on_canvas.loadFromFile("assets/A29861.jpg")) {}

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

	//we set textures to the sprites
		spr_icon_pencil_selection.setTexture(          tex_icon_pencil_selection);
		spr_icon_equal_character_selection.setTexture( tex_icon_equal_character_selection);
		spr_icon_resize.setTexture(                    tex_icon_resize);
		spr_icon_save.setTexture(                      tex_icon_save);
		spr_icon_square_selection.setTexture(          tex_icon_square_selection);
		spr_icon_wand_selection.setTexture(            tex_icon_wand_selection);
		spr_icon_folder.setTexture(                    tex_icon_folder);
		spr_icon_options.setTexture(                   tex_icon_options);
		spr_icon_resize_b.setTexture(                  tex_icon_resize_b);
		spr_icon_spacing.setTexture(                   tex_icon_spacing);

	//sound (yeah, there's sound in this software...)
		sf::SoundBuffer buffer_minimal_click;
		if(!buffer_minimal_click.loadFromFile("assets/minimal_clickb.wav")) {}
		sf::Sound sound_minimal_click;
		sound_minimal_click.setBuffer(buffer_minimal_click);
		sound_minimal_click.setVolume(15);

	//upper navigation bar
		navigation_bar_txt upper_toolbar(sf::Vector2i((int)window.getSize().x / 2, (int)window.getSize().y * 0.02), sf::Color(13, 13, 13), 8, 8, "centered", "horizontal", sound_minimal_click);
		upper_toolbar.list_of_buttons.push_back( new button_text(sf::Vector2i(1, 1), "unknow", "pressed_6on_1", font_pixel, sf::Color(25, 25, 25), 5) );
		upper_toolbar.list_of_buttons.push_back( new button_text(sf::Vector2i(1, 1), "file_text_0.txt", "pr6essed_button_1", font_pixel, sf::Color(25, 25, 25), 5));
		//upper_toolbar.list_of_buttons.push_back( new button_text(sf::Vector2i(1, 1), "file_text_1.txt", "press3ed_button_1", font_pixel, sf::Color(25, 25, 25), 5));
		//upper_toolbar.list_of_buttons.push_back( new button_text(sf::Vector2i(1, 1), "file_text_2.txt", "presse3d_button_1", font_pixel, sf::Color(25, 25, 25), 5) );
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

	//canvas button system below
		int  canvas_button_pos_x = 0;
		int  canvas_button_pos_y = 0;
		bool moving_canvas_button = false;
		button_image canvas_button_image = button_image(sf::Vector2i(canvas_button_pos_x, canvas_button_pos_y), spr_icon_resize_b, "resize");

	new_canvas.update_back_lineas(sf::Vector2i(cell_size_x,cell_size_y));

    //the main loop of the display system. Yet more optimization is needed with the cpu usage...
	sf::Clock clock;while (window.isOpen())
	{

		//std::cout << "0\n";

		clock.restart();

		//currently zoom is disabled with this option from here
		//view1.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
		hud_view.setSize(  sf::Vector2f(window.getSize().x, window.getSize().y));
		hud_view.setCenter(window.getSize().x/2, window.getSize().y/2);

		upper_toolbar.pos = sf::Vector2i((int)window.getSize().x / 2, (int)window.getSize().y * 0.02);
		main_toolbar.pos  = sf::Vector2i((int)window.getSize().x / 2, (int)window.getSize().y - (int)window.getSize().y * 0.15);
		upper_toolbar.update();
		main_toolbar.update();

		//input management (part 1)
		window.setView(canvas_view);
		mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		window.setView(hud_view);
		hud_mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

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

		//std::cout << "1\n";
		canvas_button_pos_x = displacement_x + new_canvas.size_x * cell_size_x + 6;
		canvas_button_pos_y = displacement_y + new_canvas.size_y * cell_size_y + 6;

		//we move the corner button
		if (moving_canvas_button)
		{
			canvas_button_pos_x       += ( - prev_mouse_position + mouse_position).x;
			canvas_button_image.pos.x += ( - prev_mouse_position + mouse_position).x;
			canvas_button_pos_y       += ( - prev_mouse_position + mouse_position).y;
			canvas_button_image.pos.y += ( - prev_mouse_position + mouse_position).y;

			canvas_button_image.spr.setPosition(canvas_button_image.pos.x, canvas_button_image.pos.y);

			increment_decrement_vector =                               cell_location((sf::Vector2i)mouse_position         - displacement_v , new_canvas, cell_size_x, cell_size_y);
			increment_decrement_vector = increment_decrement_vector - (cell_location((sf::Vector2i)initial_mouse_position - displacement_v , new_canvas, cell_size_x, cell_size_y));
		}
		else
		{
			canvas_button_image.pos.x = canvas_button_pos_x;
			canvas_button_image.pos.y = canvas_button_pos_y;
			canvas_button_image.spr.setPosition(canvas_button_image.pos.x, canvas_button_image.pos.y);
		}

		//std::cout << "2\n";
		//events happen here in part
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
				//view1.setSize(initial_size_x * zoom, initial_size_y * zoom);
				canvas_view.setSize(sf::Vector2f(window.getSize().x * zoom, window.getSize().y * zoom));
			}
		}

		//std::cout << "3\n";
		//button handling
		if (left_mouse_button_is_down)
		{

			string index;
			int    index_esto = -1;

			if (!moving_canvas_button && !moving_selection_around)
			{
				index      = main_toolbar.check_click(    (sf::Vector2i)hud_mouse_position);
				index_esto = upper_toolbar.index_by_click((sf::Vector2i)hud_mouse_position);
			}
			else
			{

			}

			//means we've selected something from the toolbar
			if (!index.empty())
			{
				if (index == "pencil_mode")     { selection_mode = 0; }
				if (index == "square_mode")     { selection_mode = 1; }
				if (index == "wand_mode")       { selection_mode = 2; }
				if (index == "similarity_mode") { selection_mode = 3; }

				if (index == "save" && left_mouse_button_just_down)
				{ 
					if (new_canvas.file_route.empty())
					{
						new_canvas.file_route = select_file(false);
						new_canvas.set_name_from_path(new_canvas.file_route);
					}
					new_canvas.save_to(new_canvas.file_route);
					new_canvas.set_name_from_path(new_canvas.file_route);

					canvases[active_canvas_index]->file_route  = new_canvas.file_route;
					canvases[active_canvas_index]->canvas_name = new_canvas.canvas_name;
				}

				if (index == "open_file" && left_mouse_button_just_down)
				{
					std::string to_loaaaad = select_file(true);

					if (!to_loaaaad.empty())
					{
						new_canvas.load_text_file(to_loaaaad);
					}

					canvases[active_canvas_index]->file_route  = new_canvas.file_route;
					canvases[active_canvas_index]->canvas_name = new_canvas.canvas_name;
				}
			}
			//means we've clicked one of the tabs
			else if (index_esto != -1 && left_mouse_button_just_down)
			{
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
			}
			//in case we just clicked the movement button a sound is played
			else if (canvas_button_image.is_inside((sf::Vector2i)mouse_position) && left_mouse_button_just_down)
			{
				initial_mouse_position = mouse_position;
				moving_canvas_button = true;
				sound_minimal_click.play();
			}
			//click happened inside of the canvas
			else if (!out_of_canvas)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
				{
					new_canvas.set_char_selected(new_canvas.cell_letters[cell_location_vector.y][cell_location_vector.x]);
				}
				else
				{
					//selection mode, if adding true values or false values...
					if (left_mouse_button_just_down)
					{
						//unnecesary violence
						selection_value = !new_canvas.activ_cells[cell_location_vector.y][cell_location_vector.x];
					}
					//pencil toggle selection
					if (selection_mode == 0 && !moving_canvas_button)
					{
					//stuff
					new_canvas.activ_cells[cell_location_vector.y][cell_location_vector.x] = selection_value;
					}
					//square toggle selection mode
					if (selection_mode == 1 && !moving_canvas_button)
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
					if (selection_mode == 2 && left_mouse_button_just_down && !moving_canvas_button)
					{
					if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || !sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
					{
						new_canvas.deselect_all();
					}
					new_canvas.select_bucket(cell_location_vector);
					}
					//wand selection mode
					if (selection_mode == 3 && left_mouse_button_just_down && !moving_canvas_button)
					{
					char selecion_character = new_canvas.cell_letters[cell_location_vector.y][cell_location_vector.x];
					if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || !sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
					{
						new_canvas.deselect_all();
					}
					new_canvas.equal_character_selection(selecion_character);
					}
				}

			}
		}
		if (left_mouse_button_just_up)
		{
			if (moving_canvas_button)
			{
				moving_canvas_button = false;
				new_canvas.resize(increment_decrement_vector.x, increment_decrement_vector.y);
				new_canvas.update_back_lineas(sf::Vector2i(cell_size_x,cell_size_y));
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
		//text input management, pretty much it only works when something in the canvas is selected...
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

					if(event.text.unicode == '\b')
					{
						new_canvas.set_char_selected(value_to_add);
						selection_position = new_canvas.first_position_selection();
						new_canvas.deselect_all();
						new_canvas.activ_cells[selection_position.y][selection_position.x - 1] = true;
					}
					else
					{
						selection_position = new_canvas.first_position_selection();

						if (selection_position.y+1 < new_canvas.size_y)
						{
							if (selection_position.x+1 < new_canvas.size_x)
							{
								value_to_add = event.text.unicode;
								new_canvas.set_char_selected(value_to_add);
								new_canvas.deselect_all();
								new_canvas.activ_cells[selection_position.y][selection_position.x + 1] = true;
							}
							else
							{
								value_to_add = event.text.unicode;
								new_canvas.set_char_selected(value_to_add);
								new_canvas.deselect_all();
								new_canvas.activ_cells[selection_position.y + 1][0] = true;
							}
						}
						else
						{
							if (selection_position.x+1 < new_canvas.size_x)
							{
								value_to_add = event.text.unicode;
								new_canvas.set_char_selected(value_to_add);
								new_canvas.deselect_all();
								new_canvas.activ_cells[selection_position.y][selection_position.x + 1] = true;
							}
							else
							{
								value_to_add = event.text.unicode;
								new_canvas.set_char_selected(value_to_add);
							}
						}
					}
				}
			}
		}

		//std::cout << "5\n";
		//this parts adds usability to the keys in order to move the cursor if it's just one selected
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
		//system to pan the canvas around the view OR move a selection around
		moving_selection_around = false;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
			{
				moving_selection_around = true;
			}
			else
			{
				displacement_x += mouse_position.x - prev_mouse_position.x;
				displacement_y += mouse_position.y - prev_mouse_position.y;
			}
		}
		//used to clear the selection
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			//this deletes stuff
			new_canvas.deselect_all();
		}

		//std::cout << "6\n";
		//we take care of the selection thing
		if (moving_selection_around && !prev_moving_selection_around)
		{
			//D: :DD  :D:D D: D: DDDD: ::DDD:D::D::D:
			std::cout << "started =^3^=\n";
			drag_and_drop_starting_point = (sf::Vector2i)mouse_position;
			new_canvas.drag_drop_activ_cells = new_canvas.activ_cells;
			new_canvas.drag_drop_cell_letters = new_canvas.cell_letters;
		}
		else if (moving_selection_around && prev_moving_selection_around)
		{
			//doing thiiiiiiiiiiz 7u7 heheheheh
		}
		else if (!moving_selection_around && prev_moving_selection_around)
		{
			//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
			std::cout << "finihed ? :$\n";

			new_canvas.set_char_selected(' ');
			new_canvas.deselect_all();

			new_canvas.overlay_drag_and_drop(

			new_canvas.drag_drop_activ_cells,
			new_canvas.drag_drop_cell_letters,
			(
				cell_location(
				((sf::Vector2i)mouse_position - (sf::Vector2i)drag_and_drop_starting_point), 
				new_canvas, cell_size_x, cell_size_y)
			)

			);
		}


		//std::cout << "7\n";
		//the drawing part of the loop, currently everything is drawn in the 'windows' object
		window.clear();

			window.setView(canvas_view);
			window.draw(text_consolas); //eeh ?

			//std::cout << "7.0\n";
			//draw_grid(      window, new_canvas, displacement_x, displacement_y, cell_size_x, cell_size_y);

			//new_canvas.update_back_lineas(sf::Vector2i(cell_size_x,cell_size_y));
			//new_canvas.background_lineas.setPosition(displacement_x, displacement_y);
			window.draw(new_canvas.background_lineas);
			//window.draw(new_canvas.background_lineas, &oil_on_canvas);
			//std::cout << "7.1\n";
			draw_selected(  window, new_canvas, displacement_x, displacement_y, cell_size_x, cell_size_y);
			//std::cout << "7.2\n";
			if (moving_selection_around && prev_moving_selection_around)
			{
				sf::Vector2i to_insert;
				to_insert = ((sf::Vector2i)drag_and_drop_starting_point + (sf::Vector2i)mouse_position);
				to_insert = ((sf::Vector2i)mouse_position - (sf::Vector2i)drag_and_drop_starting_point);

				draw_drag_and_drop(window, new_canvas, displacement_x, displacement_y, cell_size_x, cell_size_y, to_insert, new_canvas);
			}

			draw_characters(window, new_canvas, displacement_x + 10, displacement_y, cell_size_x, cell_size_y, text_consolas);
			//std::cout << "7.3\n";
			window.draw(canvas_button_image.spr);
			//std::cout << "7.4\n";
			if (moving_canvas_button) draw_new_canvas_size(window, new_canvas, increment_decrement_vector, displacement_x, displacement_y, cell_size_x, cell_size_y);

			window.setView(hud_view);

			//std::cout << "7.5\n";
			main_toolbar.render( window, (sf::Vector2i)hud_mouse_position);
			//std::cout << "7.6\n";
			for (int i = 0; i< upper_toolbar.list_of_buttons.size();i++)
			{upper_toolbar.list_of_buttons[i]->str = canvases[i]->canvas_name;}
			upper_toolbar.update();
			upper_toolbar.render(window, (sf::Vector2i)hud_mouse_position);
			//std::cout << "7.7\n";
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
			//std::cout << "7.8\n";
			draw_text_over_toobox_up(window, string_cell_upper_toolbox, text_pixel, font_pixel);
			//std::cout << "7.9\n";
			if (main_toolbar.check_click((sf::Vector2i)hud_mouse_position) != "" && main_toolbar.check_click((sf::Vector2i)hud_mouse_position) != "clicked the toolbar...")
			{
				std::string texti_pixel;
				texti_pixel = main_toolbar.check_click((sf::Vector2i)hud_mouse_position);
				draw_text_over_toobox_bottom(window, texti_pixel, text_pixel, font_pixel);
			}

		//std::cout << "8\n";

		//window.setView(view1);
		window.display();

		//prev variables
		prev_moving_selection_around   = moving_selection_around;
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


