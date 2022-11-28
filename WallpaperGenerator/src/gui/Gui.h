#pragma once
#include <raylib.h>
#include <map>
#include <string>
class Gui
{
private:
public:

	Texture2D preview_texture;

	std::map<std::string, float> user_values;

	int kWindowWidth;
	int kWindowHeight;

	// Main menu
	Rectangle create_rect;
	Rectangle view_rect;

	// Create menu
	Rectangle flowfield_rect;
	Rectangle shapes_rect;

	// Generate
	Rectangle preview_rect;
	Rectangle shader_checkbox;
	Rectangle blend_slider;
	Rectangle save_image;
	Rectangle reset_image;
	Rectangle update_settings;
	Rectangle reset_settings;

	// Flowfield user settings
	Rectangle window_width_rect;
	Rectangle window_height_rect;
	Rectangle scale_rect;
	Rectangle seed_rect;
	Rectangle flowfield_strength_rect;
	Rectangle particle_count_rect;
	Rectangle particle_speed_rect;
	Rectangle particle_size_rect;
	Rectangle particle_strength_rect;
	Rectangle noise_height_rect;
	Rectangle noise_detail_rect;
	Rectangle x_mult_rect;
	Rectangle y_mult_rect;
	Rectangle z_mult_rect;
	Rectangle z_rect;

	Vector2 scroll_pos;

	enum Menu
	{
		MAIN = 0,
		CREATE,
		GENERATOR
	};

	enum Algorithm
	{
		NONE = 0,
		FLOWFIELD,
		SHAPES
	};

	int active_menu = Menu::MAIN;
	int active_algorithm = Algorithm::NONE;

	Gui(int kWindowWidth, int kWindowHeight, std::map<std::string, float>& user_values);
	void Draw();
	void Update(const Texture2D &preview_texture);
	void MainMenuScreen();
	void CreateScreen();
	void FlowfieldScreen(std::map<std::string, float> &user_values);
	void ShapesScreen();

};

