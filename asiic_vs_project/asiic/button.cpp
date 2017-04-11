#include "button.h"
#include <string>

button::button(int in_x, int in_y, int in_width, int in_height, std::string in_text)
{
	x = in_x;
	y = in_y;
	width  = in_width;
	height = in_height;
	text = in_text;
}

bool button::is_inside(int in_x, int in_y)
{
	if (in_x > x && in_x < x + width)
	{
		if (in_y > y && in_y < y + height)
		{
			return true;
		}
	}
	return false;
}
