#pragma once

#include <string>

class button
{
public:
	button(int, int, int, int, std::string);
	bool is_inside(int, int);

	int x;
	int y;
	int width;
	int height;
	std::string text;

};

