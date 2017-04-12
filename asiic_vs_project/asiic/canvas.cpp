#include "canvas.h"
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

canvas::canvas(int input_size_x, int input_size_y) {

	using namespace std;

	size_x = input_size_x;
	size_y = input_size_y;

	//char canvas_array[10][10];

	list<list<string>> text;

	//vector<vector<bool>> activ_cells;
	activ_cells.resize(input_size_y, vector<bool>(input_size_x, false));
	tmp_activ_cells.resize(input_size_y, vector<short>(input_size_x, 0));
	cell_letters.resize(input_size_y, vector<char>(input_size_x, ' '));

	
	//for (int i = 0; i < input_size_x; i++) {
		//vector<bool> buffer;
		//for (int j = 0; j < input_size_y; j++) {
			//buffer.push_back(false);
			//cell_letters[i][j] = ' ';
			//}
		//activ_cells.push_back(buffer);
		//}
	
}

void canvas::deselect_all(){

	for (int i = 0; i < activ_cells.size(); i++) {
		for (int j = 0; j < activ_cells[0].size(); j++) {
			activ_cells[i][j] = false;
		}
	}
}

void canvas::set_char_selected(int input_char) {

	for (int i = 0; i < activ_cells.size(); i++) {
		for (int j = 0; j < activ_cells[0].size(); j++) {
			if (activ_cells[i][j]) {
				cell_letters[i][j] = (char)input_char;
			}
		}
	}
}

void canvas::save_to(std::string file_name) {

	std::string text = "";

	for (int i = 0; i < cell_letters.size(); i++) {
		for (int j = 0; j < cell_letters[0].size(); j++) {

			text += cell_letters[i][j];
		}

		text += "\n";
	}

	//save this shit
	std::ofstream out(file_name);
	out << text;
	out.close();

	return;

}

void canvas::set_square_selection_temporal(sf::Vector2i starting_point, sf::Vector2i ending_point, bool value) {

	int dif_x = (//std::abs(
		ending_point.x - starting_point.x);
	int dif_y = (//std::abs(
		ending_point.y - starting_point.y);

	//es necesario llamar esto en bucle ? ugh, diría que no...
	tmp_activ_cells.clear();
	tmp_activ_cells.resize(size_y, std::vector<short>(size_x, 0));

	//for (int i = 0; i < dif_x; i++)
	//{
	//	for (int j = 0; j < dif_y; j++)
	//	{
	//		tmp_activ_cells[j+starting_point.y][i+starting_point.x] = 1;
	//		if (value) tmp_activ_cells[j+starting_point.y][i+starting_point.x] = 2;
	//	}
	//}

	//std::cout << "angel malo";
	
	std::cout << " == " << dif_y << " _ " << dif_x;

	for (int i = 0; i < std::abs(dif_y) + 1; i++)
	{	
		if (dif_x > 0) {
			for (int j = 0; j < std::abs(dif_x) + 1; j++)
			{
				if (dif_y > 0) {

					std::cout << " a " << (i+starting_point.y) << " " << (j+starting_point.x);

					tmp_activ_cells[           i+starting_point.y][j+starting_point.x] = 1;
					if (value) tmp_activ_cells[i+starting_point.y][j+starting_point.x] = 2;

				}
				else
				{
					std::cout << " b " << (-i+starting_point.y) << " " << (j+starting_point.x);

					tmp_activ_cells[           -i+starting_point.y][+j+starting_point.x] = 1;
					if (value) tmp_activ_cells[-i+starting_point.y][+j+starting_point.x] = 2;

				}
			}
		}
		else
		{
			for (int j = 0; j < std::abs(dif_x) + 1; j++)
			{
				if (dif_y > 0) {

					std::cout << " c " << (-i+starting_point.y) << " " << (-j+starting_point.x);

					tmp_activ_cells[           i+starting_point.y][-j+starting_point.x] = 1;
					if (value) tmp_activ_cells[i+starting_point.y][-j+starting_point.x] = 2;

				}
				else
				{
					std::cout << " d " << (-i+starting_point.y) << " " << (+j+starting_point.x);

					tmp_activ_cells[           -i+starting_point.y][-j+starting_point.x] = 1;
					if (value) tmp_activ_cells[-i+starting_point.y][-j+starting_point.x] = 2;

				}
			}
		}
	}

	//std::cout << " x " << ending_point.x << " y " << ending_point.y;

	tmp_activ_cells[           ending_point.y][ending_point.x] = 1;
	if (value) tmp_activ_cells[ending_point.y][ending_point.x] = 2;

}

//std::vector<std::vector<char>> canvas::overlay_short_matrix(std::vector<std::vector<short>> input_matrix) {
void canvas::overlay_short_matrix(std::vector<std::vector<short>> input_matrix) {

	std::cout << " \n activ_cells x = ";

	std::cout << activ_cells.size()     << " y = ";
	std::cout << activ_cells[0].size()  << " input_matrix x =";
	std::cout << input_matrix.size()    << " y= ";
	std::cout << input_matrix[0].size() << " \n";

	
	for (int i = 0; i < input_matrix.size(); i++)
	{
		for (int j = 0; j < input_matrix[0].size(); j++)
		{
			
			//std::cout << " => " << i << "   " << j << "\n";

			if (input_matrix[i][j] == 2) { activ_cells[i][j] = true; }
			if (input_matrix[i][j] == 1) { activ_cells[i][j] = false; }
		}
	}

	std::cout << "ooo";
}

void canvas::clear_short_matrix()
{
	tmp_activ_cells.clear();
	tmp_activ_cells.resize(size_x, std::vector<short>(size_y, 0));
}

int canvas::return_ammount_selected() {
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

sf::Vector2i canvas::first_position_selection() {
	
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
	//return to_return;
}