#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class canvas
{
public:
	canvas(int, int, std::string);

	void update_back_lineas(sf::Vector2i);

	void deselect_all();
	void set_char_selected(int);
	void save_to(std::string);
	void load_text_file(std::string);
	void center_canvas_in_window();

	void set_square_selection_temporal(sf::Vector2i, sf::Vector2i, bool);
	void overlay_short_matrix(std::vector<std::vector<short> >);
	void clear_short_matrix();

	int return_ammount_selected();
	sf::Vector2i first_position_selection();
	void select_bucket(sf::Vector2i initial_point);
	void resize(int, int);
	void equal_character_selection(char);
	void set_name_from_path(std::string);
	int size_x, size_y;

	void overlay_drag_and_drop(std::vector<std::vector<bool> >, std::vector<std::vector<char> >, sf::Vector2i);
	std::string canvas_name;
	std::string file_route;

	sf::VertexArray background_lineas;

	std::vector<std::vector<bool> >  activ_cells;
	std::vector<std::vector<bool> >  drag_drop_activ_cells;
	std::vector<std::vector<char> >  drag_drop_cell_letters;

	std::vector<std::vector<short> > tmp_activ_cells;
	std::vector<std::vector<char> >  cell_letters;

	std::vector<std::vector<bool> >  buffer_activ_cells;
	std::vector<std::vector<short> > buffer_tmp_activ_cells;
	std::vector<std::vector<char> >  buffer_cell_letters;
};
