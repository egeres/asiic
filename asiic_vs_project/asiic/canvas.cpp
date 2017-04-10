#include "canvas.h"
#include <list>
#include <vector>
#include <string>

canvas::canvas(int input_size_x, int input_size_y) {

	using namespace std;

	size_x = input_size_x;
	size_y = input_size_y;

	//char canvas_array[10][10];

	list<list<string>> text;

	//vector<vector<bool>> activ_cells;
	activ_cells.resize(input_size_y, vector<bool>(input_size_x, false));

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