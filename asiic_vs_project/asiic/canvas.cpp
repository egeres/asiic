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

//canvas constructor
canvas::canvas(int input_size_x, int input_size_y)
{

	using namespace std;

	size_x = input_size_x;
	size_y = input_size_y;

	//char canvas_array[10][10];

	list<list<string>> text;
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
	std::ofstream out(file_name);
	out << text;
	out.close();

	return;
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
	//std::cout << "started bucket selection...\n";
	std::vector<sf::Vector2i*> open_list;
	std::vector<sf::Vector2i*> clos_list;

	char selected_char = cell_letters[initial_point.y][initial_point.x];
	open_list.push_back(new sf::Vector2i(initial_point.x,initial_point.y));

	//std::cout << "b.0\n";


	while (open_list.size() > 0)
	{

		int asigned_size = open_list.size();
		for (int i = 0; i < asigned_size; i++)
		{

			//std::cout << "b.1\n";

			sf::Vector2i* selected_point = open_list[0];

			//std::cout << "b.2\n";

			//if ((selected_point->x >= 0 && selected_point->x <= size_x-1 && selected_point->y-1 >= 0 && selected_point->y-1 <= size_y-1) &&
			if ((selected_point->x >= 0 && selected_point->x <= activ_cells[0].size()-1 && selected_point->y-1 >= 0 && selected_point->y-1 <= activ_cells.size()-1) &&
				(cell_letters[selected_point->y-1][selected_point->x] == selected_char) && 
				(!check_inside_vector_list(open_list, sf::Vector2i(selected_point->x,selected_point->y-1))) &&
				(!check_inside_vector_list(clos_list, sf::Vector2i(selected_point->x,selected_point->y-1)))) {
				//std::cout << "b.2.0\n";

				open_list.push_back(new sf::Vector2i(selected_point->x,selected_point->y-1));
			}

			//std::cout << "b.3\n";

			//if ((selected_point->x+1 >= 0 && selected_point->x+1 <= size_x-1 && selected_point->y >= 0 && selected_point->y <= size_y-1) &&
			if ((selected_point->x+1 >= 0 && selected_point->x+1 <= activ_cells[0].size()-1 && selected_point->y >= 0 && selected_point->y <= activ_cells.size()-1) &&
				(cell_letters[selected_point->y][selected_point->x+1] == selected_char) && 
				(!check_inside_vector_list(open_list, sf::Vector2i(selected_point->x+1,selected_point->y))) &&
				(!check_inside_vector_list(clos_list, sf::Vector2i(selected_point->x+1,selected_point->y)))) {
				open_list.push_back(new sf::Vector2i(selected_point->x+1,selected_point->y));
			}

			//if ((selected_point->x >= 0 && selected_point->x <= size_x-1 && selected_point->y+1 >= 0 && selected_point->y+1 <= size_y-1) &&
			if ((selected_point->x >= 0 && selected_point->x <= activ_cells[0].size()-1 && selected_point->y+1 >= 0 && selected_point->y+1 <= activ_cells.size()-1) &&
				(cell_letters[selected_point->y+1][selected_point->x] == selected_char) && 
				(!check_inside_vector_list(open_list, sf::Vector2i(selected_point->x,selected_point->y+1))) &&
				(!check_inside_vector_list(clos_list, sf::Vector2i(selected_point->x,selected_point->y+1)))) {
				open_list.push_back(new sf::Vector2i(selected_point->x,selected_point->y+1));
			}

			//if ((selected_point->x-1 >= 0 && selected_point->x-1 <= size_x-1 && selected_point->y >= 0 && selected_point->y <= size_y-1) &&
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
	//std::cout << "started bucket selection...\n";

}

//function which overlays the temporary matrix over the original matrix activ_cells
void canvas::overlay_short_matrix(std::vector<std::vector<short>> input_matrix)
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

		//std::cout << "esto e\n";

		buffer_activ_cells     = activ_cells;
		buffer_tmp_activ_cells = tmp_activ_cells;
		buffer_cell_letters    = cell_letters;

		activ_cells.clear();
		tmp_activ_cells.clear();
		cell_letters.clear();

		activ_cells.resize(    size_y, std::vector<bool>( size_x, false));
		tmp_activ_cells.resize(size_y, std::vector<short>(size_x, 0));
		cell_letters.resize(   size_y, std::vector<char>( size_x, ' '));

		//std::cout << "resized to : "<< activ_cells[0].size() << " : " << activ_cells.size() << "\n";

		//std::cout << "resizing... 0\n";

		//std::cout << "activ_cells [" << activ_cells.size() << " " << activ_cells[0].size() << "] ";
		//std::cout << "      buffer_activ_cells [" << buffer_activ_cells.size() << " " << buffer_activ_cells[0].size() << "] \n";

		//std::cout << "activ_cells [" << activ_cells.size() << " " << activ_cells[0].size() << "] ";
		//std::cout << "      buffer_activ_cells [" << buffer_activ_cells.size() << " " << buffer_activ_cells[0].size() << "] \n";

		//std::cout << "cell_letters [" << cell_letters.size() << " " << cell_letters[0].size() << "] ";
		//std::cout << "      buffer_cell_letters [" << buffer_cell_letters.size() << " " << buffer_cell_letters[0].size() << "] \n";

		if (activ_cells.size() < buffer_activ_cells.size()) 
		{ for_size_y = activ_cells.size(); }
		else { for_size_y = buffer_cell_letters.size(); }

		if (activ_cells[0].size() < buffer_activ_cells[0].size()) 
		{ for_size_x = activ_cells[0].size(); }
		else { for_size_x = buffer_cell_letters[0].size(); }

		//std::cout << " final x = " << for_size_x << " final y = " << for_size_y;
		//std::cout << "\nresizing... 1\n";

		for (int y = 0; y < for_size_y; y++)
		{
			for (int x = 0; x < for_size_x; x++)
			{

				//std::cout << "[" << y << " " << x << "]\n";

				//std::cout << "0\n";
				//std::cout << activ_cells[y][x] << " \n";
				//std::cout << buffer_activ_cells[y][x]  << " \n";
				activ_cells[y][x]     = buffer_activ_cells[y][x];

				//std::cout << "1\n";
				//std::cout << tmp_activ_cells[y][x] << " \n";
				//std::cout << buffer_tmp_activ_cells[y][x] << " \n";
				//tmp_activ_cells[y][x] = buffer_tmp_activ_cells[y][x];

				//std::cout << "2\n";
				//std::cout << cell_letters[y][x] << " \n";
				//std::cout << buffer_cell_letters[y][x] << " \n";
				cell_letters[y][x]    = buffer_cell_letters[y][x];


			}
		}

		//std::cout << "resizing... 2\n";


		buffer_activ_cells.clear();
		buffer_tmp_activ_cells.clear();
		buffer_cell_letters.clear();

		//std::cout << "resizing... 3\n";

	}
	else
	{
		//std::cout << "cannot resize that...";
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