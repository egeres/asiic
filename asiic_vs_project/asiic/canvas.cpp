#include "canvas.h"
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>

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

void canvas::select_bucket(sf::Vector2i initial_point) {

	std::cout << " petar.. xD\n\n\n\n";

	std::vector<sf::Vector2i*> test;
	test.push_back(new sf::Vector2i(1,2));

	std::cout << test[0]->x << " : " << test[0]->y;

	std::cout << ".  .  .  .\n";

	std::cout << " caso 0 = " << ( std::find(test.begin(), test.end(), new sf::Vector2i(1,2)) != test.end() ) << "\n";
	std::cout << "      0 = " << (check_inside_vector_list(test,           sf::Vector2i(1,2))) << "\n";
	std::cout << " caso 1 = " << ( std::find(test.begin(), test.end(), new sf::Vector2i(2,2)) != test.end() ) << "\n";
	std::cout << "      1 = " << (check_inside_vector_list(test,           sf::Vector2i(2,2))) << "\n";
	std::cout << " caso 2 = " << ( std::find(test.begin(), test.end(), new sf::Vector2i(4,2)) != test.end() ) << "\n";
	std::cout << "      2 = " << (check_inside_vector_list(test,           sf::Vector2i(4,2))) << "\n";
	std::cout << " caso 3 = " << ( std::find(test.begin(), test.end(), new sf::Vector2i(4,9)) != test.end() ) << "\n";
	std::cout << "      3 = " << (check_inside_vector_list(test,           sf::Vector2i(4,9))) << "\n";

	std::cout << ".  .  .  .\n";

	std::vector<sf::Vector2i*> open_list;
	std::vector<sf::Vector2i*> clos_list;

	//get first elements in the open list
	char selected_char = cell_letters[initial_point.y][initial_point.x];
	int current_index = 0;
	//check out surroundings

	/*
	if (cell_letters[initial_point.y-1][initial_point.x] == selected_char){
		open_list.push_back(new sf::Vector2i(initial_point.x,initial_point.y-1));
	}
	if (cell_letters[initial_point.y][initial_point.x+1] == selected_char){
		open_list.push_back(new sf::Vector2i(initial_point.x+1,initial_point.y));
	}
	if (cell_letters[initial_point.y+1][initial_point.x] == selected_char){
		open_list.push_back(new sf::Vector2i(initial_point.x,initial_point.y+1));
	}
	if (cell_letters[initial_point.y][initial_point.x-1] == selected_char){
		open_list.push_back(new sf::Vector2i(initial_point.x-1,initial_point.y));
	}
	*/

	std::cout << "0a\n";

	//std::cout << "a" << "\n";
	//std::cout << (initial_point.x > 0 && initial_point.x < size_x && initial_point.y-1 > 0 && initial_point.y-1 < size_y) << "\n";
	//std::cout << (cell_letters[initial_point.y-1][initial_point.x] == selected_char) << "\n";
	//std::cout << ( std::find(open_list.begin(), open_list.end(), (new sf::Vector2i(initial_point.x,initial_point.y-1))) == open_list.end() ) << "\n";
	//std::cout << ( std::find(clos_list.begin(), clos_list.end(), (new sf::Vector2i(initial_point.x,initial_point.y-1))) == clos_list.end() ) << "\n";
	//std::cout << "a" << "\n";

	if ((initial_point.x >= 0 && initial_point.x <= size_x-1 && initial_point.y-1 >= 0 && initial_point.y-1 <= size_y-1) &&
		(cell_letters[initial_point.y-1][initial_point.x] == selected_char) && 
		//( std::find(open_list.begin(), open_list.end(), (new sf::Vector2i(initial_point.x,initial_point.y-1))) == open_list.end() ) &&
		//( std::find(clos_list.begin(), clos_list.end(), (new sf::Vector2i(initial_point.x,initial_point.y-1))) == clos_list.end() )) {
		(!check_inside_vector_list(open_list, sf::Vector2i(initial_point.x,initial_point.y-1))) &&
		(!check_inside_vector_list(clos_list, sf::Vector2i(initial_point.x,initial_point.y-1)))) {
		open_list.push_back(new sf::Vector2i(initial_point.x,initial_point.y-1));
		std::cout << "added...\n";
	}

	std::cout << "0b\n";

	if ((initial_point.x+1 >= 0 && initial_point.x+1 <= size_x-1 && initial_point.y >= 0 && initial_point.y <= size_y-1) &&
		(cell_letters[initial_point.y][initial_point.x+1] == selected_char) && 
		//( std::find(open_list.begin(), open_list.end(), (new sf::Vector2i(initial_point.x+1,initial_point.y))) == open_list.end() ) &&
		//( std::find(clos_list.begin(), clos_list.end(), (new sf::Vector2i(initial_point.x+1,initial_point.y))) == clos_list.end() )) {
		(!check_inside_vector_list(open_list, sf::Vector2i(initial_point.x+1,initial_point.y))) &&
		(!check_inside_vector_list(clos_list, sf::Vector2i(initial_point.x+1,initial_point.y)))) {
		open_list.push_back(new sf::Vector2i(initial_point.x+1,initial_point.y));
		std::cout << "added...\n";
	}

	std::cout << "0c\n";

	if ((initial_point.x >= 0 && initial_point.x <= size_x-1 && initial_point.y+1 >= 0 && initial_point.y+1 <= size_y-1) &&
		(cell_letters[initial_point.y+1][initial_point.x] == selected_char) && 
		//( std::find(open_list.begin(), open_list.end(), (new sf::Vector2i(initial_point.x,initial_point.y+1))) == open_list.end() ) &&
		//( std::find(clos_list.begin(), clos_list.end(), (new sf::Vector2i(initial_point.x,initial_point.y+1))) == clos_list.end() )) {
		(!check_inside_vector_list(open_list, sf::Vector2i(initial_point.x,initial_point.y+1))) &&
		(!check_inside_vector_list(clos_list, sf::Vector2i(initial_point.x,initial_point.y+1)))) {
		open_list.push_back(new sf::Vector2i(initial_point.x,initial_point.y+1));
		std::cout << "added...\n";
	}

	std::cout << "0d\n";

	if ((initial_point.x-1 >= 0 && initial_point.x-1 <= size_x-1 && initial_point.y >= 0 && initial_point.y <= size_y-1) &&
		(cell_letters[initial_point.y][initial_point.x-1] == selected_char) && 
		//( std::find(open_list.begin(), open_list.end(), (new sf::Vector2i(initial_point.x-1,initial_point.y))) == open_list.end() ) &&
		//( std::find(clos_list.begin(), clos_list.end(), (new sf::Vector2i(initial_point.x-1,initial_point.y))) == clos_list.end() )) {
		(!check_inside_vector_list(open_list, sf::Vector2i(initial_point.x-1,initial_point.y))) &&
		(!check_inside_vector_list(clos_list, sf::Vector2i(initial_point.x-1,initial_point.y)))) {
		open_list.push_back(new sf::Vector2i(initial_point.x-1,initial_point.y));
		std::cout << "added...\n";
	}

	std::cout << "1\n";

	std::cout << initial_point.x << " - " << initial_point.y << "\n";

	activ_cells[initial_point.y][initial_point.x] = true;
	
	std::cout << "1b\n";

	clos_list.push_back(new sf::Vector2i(initial_point.x,initial_point.y));

	std::cout << "1c " << open_list.size() << "\n";

	//open_list.erase(open_list.begin() + 1 );

	std::cout << "2\n";

	while (open_list.size() > 0)
	{

		std::cout << "2a list size = " << open_list.size() << "\n";

		int asigned_size = open_list.size();

		for (int i = 0; i < asigned_size; i++)
		{

			if (i==100) { cell_letters[-1][-1] = 'i'; }

			//check out surroundings
			//sf::Vector2i* selected_point = open_list[i];
			sf::Vector2i* selected_point = open_list[0];

			std::cout << "2aa  i = " << i << "      x=" << selected_point->x << " y=" << selected_point->y << "\n";


			/*
			if (cell_letters[initial_point.y][initial_point.x+1] == selected_char){open_list.push_back(new sf::Vector2i(selected_point->x+1,selected_point->y));}
			if (cell_letters[initial_point.y-1][initial_point.x] == selected_char){open_list.push_back(new sf::Vector2i(selected_point->x,selected_point->y-1));}
			if (cell_letters[initial_point.y+1][initial_point.x] == selected_char){open_list.push_back(new sf::Vector2i(selected_point->x,selected_point->y+1));}
			if (cell_letters[initial_point.y][initial_point.x-1] == selected_char){open_list.push_back(new sf::Vector2i(selected_point->x-1,selected_point->y));}
			*/

			//std::cout << "2ab\n";

			if ((selected_point->x >= 0 && selected_point->x <= size_x-1 && selected_point->y-1 >= 0 && selected_point->y-1 <= size_y-1) &&
				(cell_letters[selected_point->y-1][selected_point->x] == selected_char) && 
				//( std::find(open_list.begin(), open_list.end(), (new sf::Vector2i(selected_point->x,selected_point->y-1))) == open_list.end() ) &&
				//( std::find(clos_list.begin(), clos_list.end(), (new sf::Vector2i(selected_point->x,selected_point->y-1))) == clos_list.end() )) {
				(!check_inside_vector_list(open_list, sf::Vector2i(selected_point->x,selected_point->y-1))) &&
				(!check_inside_vector_list(clos_list, sf::Vector2i(selected_point->x,selected_point->y-1)))) {
				open_list.push_back(new sf::Vector2i(selected_point->x,selected_point->y-1));
			}
			//std::cout << "2ac\n";
			if ((selected_point->x+1 >= 0 && selected_point->x+1 <= size_x-1 && selected_point->y >= 0 && selected_point->y <= size_y-1) &&
				(cell_letters[selected_point->y][selected_point->x+1] == selected_char) && 
				//( std::find(open_list.begin(), open_list.end(), (new sf::Vector2i(selected_point->x+1,selected_point->y))) == open_list.end() ) &&
				//( std::find(clos_list.begin(), clos_list.end(), (new sf::Vector2i(selected_point->x+1,selected_point->y))) == clos_list.end() )) {
				(!check_inside_vector_list(open_list, sf::Vector2i(selected_point->x+1,selected_point->y))) &&
				(!check_inside_vector_list(clos_list, sf::Vector2i(selected_point->x+1,selected_point->y)))) {
				open_list.push_back(new sf::Vector2i(selected_point->x+1,selected_point->y));
			}
			//std::cout << "2ad\n";

			if ((selected_point->x >= 0 && selected_point->x <= size_x-1 && selected_point->y+1 >= 0 && selected_point->y+1 <= size_y-1) &&
				(cell_letters[selected_point->y+1][selected_point->x] == selected_char) && 
				//( std::find(open_list.begin(), open_list.end(), (new sf::Vector2i(selected_point->x,selected_point->y+1))) == open_list.end() ) &&
				//( std::find(clos_list.begin(), clos_list.end(), (new sf::Vector2i(selected_point->x,selected_point->y+1))) == clos_list.end() )) {
				(!check_inside_vector_list(open_list, sf::Vector2i(selected_point->x,selected_point->y+1))) &&
				(!check_inside_vector_list(clos_list, sf::Vector2i(selected_point->x,selected_point->y+1)))) {
				open_list.push_back(new sf::Vector2i(selected_point->x,selected_point->y+1));
			}
			//std::cout << "2ae\n";

			if ((selected_point->x-1 >= 0 && selected_point->x-1 <= size_x-1 && selected_point->y >= 0 && selected_point->y <= size_y-1) &&
				(cell_letters[selected_point->y][selected_point->x-1] == selected_char) && 
				//( std::find(open_list.begin(), open_list.end(), (new sf::Vector2i(selected_point->x-1,selected_point->y))) == open_list.end() ) &&
				//( std::find(clos_list.begin(), clos_list.end(), (new sf::Vector2i(selected_point->x-1,selected_point->y))) == clos_list.end() )) {
				(!check_inside_vector_list(open_list, sf::Vector2i(selected_point->x-1,selected_point->y))) &&
				(!check_inside_vector_list(clos_list, sf::Vector2i(selected_point->x-1,selected_point->y)))) {
				open_list.push_back(new sf::Vector2i(selected_point->x-1,selected_point->y));
			}
			//std::cout << "2af\n";

			//activ_cells[selected_point->y][selected_point->x] = true;
			clos_list.push_back(new sf::Vector2i(selected_point->x,selected_point->y));
			open_list.erase(open_list.begin() );
			//open_list.erase(open_list.begin() + 1 + i );
			//i++;

			//std::cout << "2ag\n";

		}

		std::cout << "22 list size     = " << open_list.size() << "\n";
		std::cout << "closed list size = " << clos_list.size() << "\n";

		//cell_letters[-1][-1] = 'i';


	}

	std::cout << "3\n";

	for (int i = 0; i < clos_list.size(); i++)
	{
		activ_cells[clos_list[i]->y][clos_list[i]->x] = true;
	}

	std::cout << "4\n";

	//cell_letters[-1][-1] = 'i';
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