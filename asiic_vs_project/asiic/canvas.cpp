#include "canvas.h"
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <string.h>
#include <sstream>

//canvas constructor
canvas::canvas(int input_size_x, int input_size_y, std::string input_name)
{
	using namespace std;

	size_x      = input_size_x;
	size_y      = input_size_y;
	canvas_name = input_name;

	list<list<string> > text;
	activ_cells.resize(    input_size_y, vector<bool>( input_size_x, false));
	tmp_activ_cells.resize(input_size_y, vector<short>(input_size_x, 0));
	cell_letters.resize(   input_size_y, vector<char>( input_size_x, ' '));
}
//turns all the activ_cells in false
void canvas::deselect_all()
{
	std::cout << "started deselecting all...\n";
	for (int i = 0; i < activ_cells.size(); i++)
	{
		for (int j = 0; j < activ_cells[0].size(); j++)
		{
			activ_cells[i][j] = false;
		}
	}
	std::cout << "finished deselecting all...\n";
}
//turns all the selected cells in that specific char
void canvas::set_char_selected(int input_char)
{
	for (int i = 0; i < activ_cells.size(); i++)
	{
		for (int j = 0; j < activ_cells[0].size(); j++)
		{
			if (activ_cells[i][j])
			{
				cell_letters[i][j] = (char)input_char;
			}
		}
	}
}
//saves the canvas cell letters inside of a txt named as the "file_name" variable
void canvas::save_to(std::string file_name)
{
	std::string text = "";

	for (int i = 0; i < cell_letters.size(); i++)
	{
		for (int j = 0; j < cell_letters[0].size(); j++)
		{
			text += cell_letters[i][j];
		}
		text += "\n";
	}

	//save this stuff
	std::ofstream out(file_name.c_str());
	out << text;
	out.close();

	return;
}

void canvas::center_canvas_in_window() {}

//loads a given file into the canvas
void canvas::load_text_file(std::string input_file_name)
{
	set_name_from_path(input_file_name);
	file_route = input_file_name;

	std::ifstream file(input_file_name.c_str());
	std::string str;

	int canvas_size_x = 0;
	int canvas_size_y = 0;

	//the first step consists on measuing
	while (std::getline(file, str))
	{
		if (str.size() > canvas_size_x) { canvas_size_x = str.size(); }
	    canvas_size_y += 1;
	}

	deselect_all();

	resize(canvas_size_x - size_x, canvas_size_y - size_y);
	file.clear();
	file.seekg(0, file.beg);
	int counter_line = 0;

	//the second step is to assign
	while (std::getline(file, str))
	{
		//we print this line's characters
		for (int i = 0; i < str.size(); i++)
		{
			if (i < str.size())
			{
				cell_letters[counter_line][i] = (char)str[i];
			}
		}

		//we print the rest of blank spaces
		for (int i = str.size(); i < canvas_size_x; i++)
		{
			cell_letters[counter_line][i] = ' ';
		}

	    counter_line+=1;
	}
}
//sets a preview to the square selection
void canvas::set_square_selection_temporal(sf::Vector2i starting_point, sf::Vector2i ending_point, bool value)
{

	int dif_x = (ending_point.x - starting_point.x);
	int dif_y = (ending_point.y - starting_point.y);

	//es necesario llamar esto en bucle ? ugh, diría que no...
	tmp_activ_cells.clear();
	tmp_activ_cells.resize(size_y, std::vector<short>(size_x, 0));

	for (int i = 0; i < std::abs(dif_y) + 1; i++)
	{
		if (dif_x > 0)
		{
			for (int j = 0; j < std::abs(dif_x) + 1; j++)
			{
				if (dif_y > 0)
				{
					tmp_activ_cells[           i+starting_point.y][j+starting_point.x] = 1;
					if (value) tmp_activ_cells[i+starting_point.y][j+starting_point.x] = 2;
				}
				else
				{
					tmp_activ_cells[           -i+starting_point.y][+j+starting_point.x] = 1;
					if (value) tmp_activ_cells[-i+starting_point.y][+j+starting_point.x] = 2;
				}
			}
		}
		else
		{
			for (int j = 0; j < std::abs(dif_x) + 1; j++)
			{
				if (dif_y > 0)
				{
					tmp_activ_cells[           i+starting_point.y][-j+starting_point.x] = 1;
					if (value) tmp_activ_cells[i+starting_point.y][-j+starting_point.x] = 2;
				}
				else
				{
					tmp_activ_cells[           -i+starting_point.y][-j+starting_point.x] = 1;
					if (value) tmp_activ_cells[-i+starting_point.y][-j+starting_point.x] = 2;
				}
			}
		}
	}

	tmp_activ_cells[           ending_point.y][ending_point.x] = 1;
	if (value) tmp_activ_cells[ending_point.y][ending_point.x] = 2;
}
//used in the function from below to check if input_2d_vector is inside of input_vector
bool check_inside_vector_list(std::vector<sf::Vector2i*> input_vector, sf::Vector2i input_2d_vector)
{
	for(int i = 0; i< input_vector.size(); i++)
	{
		if (input_vector[i]->x == input_2d_vector.x && input_vector[i]->y == input_2d_vector.y)
		{
			return true;
		}
	}
	return false;
}
//function that selects all characters by similarity and closure
void canvas::select_bucket(sf::Vector2i initial_point)
{
	std::vector<sf::Vector2i*> open_list;
	std::vector<sf::Vector2i*> clos_list;

	char selected_char = cell_letters[initial_point.y][initial_point.x];
	open_list.push_back(new sf::Vector2i(initial_point.x,initial_point.y));

	while (open_list.size() > 0)
	{
		int asigned_size = open_list.size();
		for (int i = 0; i < asigned_size; i++)
		{
			sf::Vector2i* selected_point = open_list[0];

			if ((selected_point->x >= 0 && selected_point->x <= activ_cells[0].size()-1 && selected_point->y-1 >= 0 && selected_point->y-1 <= activ_cells.size()-1) &&
				(cell_letters[selected_point->y-1][selected_point->x] == selected_char) &&
				(!check_inside_vector_list(open_list, sf::Vector2i(selected_point->x,selected_point->y-1))) &&
				(!check_inside_vector_list(clos_list, sf::Vector2i(selected_point->x,selected_point->y-1)))) {
				open_list.push_back(new sf::Vector2i(selected_point->x,selected_point->y-1));
			}

			if ((selected_point->x+1 >= 0 && selected_point->x+1 <= activ_cells[0].size()-1 && selected_point->y >= 0 && selected_point->y <= activ_cells.size()-1) &&
				(cell_letters[selected_point->y][selected_point->x+1] == selected_char) &&
				(!check_inside_vector_list(open_list, sf::Vector2i(selected_point->x+1,selected_point->y))) &&
				(!check_inside_vector_list(clos_list, sf::Vector2i(selected_point->x+1,selected_point->y)))) {
				open_list.push_back(new sf::Vector2i(selected_point->x+1,selected_point->y));
			}

			if ((selected_point->x >= 0 && selected_point->x <= activ_cells[0].size()-1 && selected_point->y+1 >= 0 && selected_point->y+1 <= activ_cells.size()-1) &&
				(cell_letters[selected_point->y+1][selected_point->x] == selected_char) &&
				(!check_inside_vector_list(open_list, sf::Vector2i(selected_point->x,selected_point->y+1))) &&
				(!check_inside_vector_list(clos_list, sf::Vector2i(selected_point->x,selected_point->y+1)))) {
				open_list.push_back(new sf::Vector2i(selected_point->x,selected_point->y+1));
			}

			if ((selected_point->x-1 >= 0 && selected_point->x-1 <= activ_cells[0].size()-1 && selected_point->y >= 0 && selected_point->y <= activ_cells.size()-1) &&
				(cell_letters[selected_point->y][selected_point->x-1] == selected_char) &&
				(!check_inside_vector_list(open_list, sf::Vector2i(selected_point->x-1,selected_point->y))) &&
				(!check_inside_vector_list(clos_list, sf::Vector2i(selected_point->x-1,selected_point->y)))) {
				open_list.push_back(new sf::Vector2i(selected_point->x-1,selected_point->y));
			}

			clos_list.push_back(new sf::Vector2i(selected_point->x,selected_point->y));
			activ_cells[selected_point->y][selected_point->x] = true;
			open_list.erase(open_list.begin() );
		}
	}
}
//function which overlays the temporary matrix over the original matrix activ_cells
void canvas::overlay_short_matrix(std::vector<std::vector<short> > input_matrix)
{
	for (int i = 0; i < input_matrix.size(); i++)
	{
		for (int j = 0; j < input_matrix[0].size(); j++)
		{
			if (input_matrix[i][j] == 2) { activ_cells[i][j] = true;  }
			if (input_matrix[i][j] == 1) { activ_cells[i][j] = false; }
		}
	}
}
//used to clear out
void canvas::clear_short_matrix()
{
	tmp_activ_cells.clear();
	tmp_activ_cells.resize(size_x, std::vector<short>(size_y, 0));
}
//counts how many active cells are in there
int canvas::return_ammount_selected()
{
	int to_return = 0;
	for (int i = 0; i < activ_cells.size(); i++)
	{
		for (int j = 0; j < activ_cells[0].size(); j++)
		{

			if (activ_cells[i][j]) { to_return++; }
		}
	}
	return to_return;
}
//returns the first position that has been selected
sf::Vector2i canvas::first_position_selection()
{
	sf::Vector2i to_return;
	for (int i = 0; i < activ_cells.size(); i++)
	{
		for (int j = 0; j < activ_cells[0].size(); j++)
		{
			if (activ_cells[i][j])
			{
				to_return.x = j;
				to_return.y = i;
				return to_return;
			}
		}
	}
}
//resizes the canvas to a new size, the input consists on two integers like -3, 6
void canvas::resize(int input_x, int input_y)
{
	if ((size_x + input_x > 0) && (size_y + input_y > 0))
	{
		int for_size_x;
		int for_size_y;

		size_x += input_x;
		size_y += input_y;

		buffer_activ_cells     = activ_cells;
		buffer_tmp_activ_cells = tmp_activ_cells;
		buffer_cell_letters    = cell_letters;

		activ_cells.clear();
		tmp_activ_cells.clear();
		cell_letters.clear();

		activ_cells.resize(    size_y, std::vector<bool>( size_x, false));
		tmp_activ_cells.resize(size_y, std::vector<short>(size_x, 0));
		cell_letters.resize(   size_y, std::vector<char>( size_x, ' '));

		if (activ_cells.size() < buffer_activ_cells.size())
		{ for_size_y = activ_cells.size(); }
		else { for_size_y = buffer_cell_letters.size(); }

		if (activ_cells[0].size() < buffer_activ_cells[0].size())
		{ for_size_x = activ_cells[0].size(); }
		else { for_size_x = buffer_cell_letters[0].size(); }

		for (int y = 0; y < for_size_y; y++)
		{
			for (int x = 0; x < for_size_x; x++)
			{
				activ_cells[y][x]     = buffer_activ_cells[y][x];
				cell_letters[y][x]    = buffer_cell_letters[y][x];
			}
		}

		buffer_activ_cells.clear();
		buffer_tmp_activ_cells.clear();
		buffer_cell_letters.clear();
	}
	else
	{
		//std::cout << "cannot resize that... ? eeh";
	}
}
//selects all the characters of the canvas given a certain input character
void canvas::equal_character_selection(char input_character)
{
	for (int i = 0; i < cell_letters.size(); i++)
	{
		for (int j = 0; j < cell_letters[0].size(); j++)
		{
			if (cell_letters[i][j] == input_character)
			{
				activ_cells[i][j] = true;
			}
		}
	}
}
//from something like "D:/cosa/stuff/file.txt", set and extract "file" as a name
void canvas::set_name_from_path(std::string input_file_name)
{
	std::istringstream ss(input_file_name);
	std::string segment;
	std::vector<std::string> seglist;

	while(std::getline(ss, segment, '\\'))
	{seglist.push_back(segment);}

	std::string step_2 = seglist.back();
	seglist.clear();
	std::istringstream ss2(step_2);

	while(std::getline(ss2, segment, '.'))
	{seglist.push_back(segment);}

	canvas_name = seglist.front();
}
//
void canvas::overlay_drag_and_drop(std::vector<std::vector<bool> > input_matrix_bool, std::vector<std::vector<char> > input_matrix_char, sf::Vector2i vec)
{
	for (int i = 0; i < input_matrix_bool.size(); i++)
	{
		for (int j = 0; j < input_matrix_bool[0].size(); j++)
		{

			if (
					((i+vec.y >= 0) && (i+vec.y < activ_cells.size())) && 
					((j+vec.x >= 0) && (j+vec.x < activ_cells[0].size()))
				)
			{
				if (input_matrix_bool[i][j])
				{
					cell_letters[i+vec.y][j+vec.x] = input_matrix_char[i][j];
				}
			}
		}
	}
}
//
void canvas::update_back_lineas(sf::Vector2i cell_size)
{

	int numero_lineas  = 0;
	numero_lineas += (size_x + 1) * 2;
	numero_lineas += (size_y + 1) * 2;
	background_lineas  = sf::VertexArray(sf::Lines, numero_lineas);

	//we update the horizontal lines
	for (int i = 0; i < size_y + 1; i++)
	{
		int x;
		int y;

		x = 0;
		y = (i + 0) * cell_size.y;
		background_lineas[i*2].position = sf::Vector2f(x, y);

		x = 1 * size_x * cell_size.x;
		y = (i + 0) * cell_size.y;
		background_lineas[i*2 + 1].position = sf::Vector2f(x, y);
	}

	//vertical lines are updated
	for (int i = 0; i < size_x + 1; i++)
	{
		int x;
		int y;

		x = (i + 0) * cell_size.x;
		y = 0;
		background_lineas[size_y+1+ i*2].position = sf::Vector2f(x, y);

		x = (i + 0) * cell_size.x;
		y = 1 * size_y * cell_size.y;
		background_lineas[size_y+1+ i*2 + 1].position = sf::Vector2f(x, y);
	}
}