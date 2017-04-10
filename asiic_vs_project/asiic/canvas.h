#pragma once

#include <vector>

class canvas
{
public:
	canvas(int, int);
	int size_x, size_y;
	std::vector<std::vector<bool>> activ_cells;
};
