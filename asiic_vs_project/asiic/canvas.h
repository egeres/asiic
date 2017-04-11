#pragma once

#include <vector>
#include <string>

class canvas
{
public:
	canvas(int, int);
	void deselect_all();
	void set_char_selected(int);
	void save_to(std::string);

	int size_x, size_y;
	std::vector<std::vector<bool>> activ_cells;
	std::vector<std::vector<char>> cell_letters;
};
