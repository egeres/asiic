#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class canvas
{
public:
	canvas(int, int);
	void deselect_all();
	void set_char_selected(int);
	void save_to(std::string);
	void set_square_selection_temporal(sf::Vector2i, sf::Vector2i, bool);
	void overlay_short_matrix(std::vector<std::vector<short> >);
	void clear_short_matrix();

	int size_x, size_y;
	std::vector<std::vector<bool> > activ_cells;
	std::vector<std::vector<short> > tmp_activ_cells;

	std::vector<std::vector<char> > cell_letters;
};
