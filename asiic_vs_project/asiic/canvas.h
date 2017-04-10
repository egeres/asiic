#pragma once

#include <vector>

class canvas
{
public:
	canvas(int, int);
	void deselect_all();
	void set_char_selected(int);

	int size_x, size_y;
	std::vector<std::vector<bool>> activ_cells;
	std::vector<std::vector<char>> cell_letters;
};
