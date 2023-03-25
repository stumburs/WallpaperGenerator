#pragma once
#include <raylib.h>
#include <map>
#include <array>
#include <string>
#include "../generator/flowfield/Flowfield.h"
#include "../generator/shapes/Shapes.h"
#include "../generator/voronoi/Voronoi.h"
class Gui
{
private:
public:

	const std::string version_number = "0.5-alpha";
	const std::string github_url = "https://github.com/stumburs/WallpaperGenerator";

	Texture2D preview_texture{};

	Flowfield f;
	Shapes s;
	Voronoi v;

	int kWindowWidth;
	int kWindowHeight;

	// Main menu
	Rectangle create_rect;
	Rectangle view_rect;
	Rectangle github_link_rect;
	Rectangle back_rect_center;
	Rectangle back_rect_corner;

	// Create menu
	Rectangle flowfield_rect;
	Rectangle shapes_rect;
	Rectangle voronoi_rect;

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
		VIEW,
		GENERATOR
	};

	enum ActiveGenerator
	{
		NONE = 0,
		FLOWFIELD,
		SHAPES,
		VORONOI
	};
	Rectangle first_setting_rect = { 110, 90, 400, 40 };

	int active_menu = Menu::MAIN;
	int active_generator = Gui::ActiveGenerator::NONE;

	Gui(int kWindowWidth, int kWindowHeight);
	void Draw();
	void Update();
	void MainMenuScreen();
	void CreateScreen();
	void ViewScreen();
	void GeneratorScreen(Generator& generator);

};
