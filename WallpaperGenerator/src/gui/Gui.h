#pragma once
#include <raylib.h>
#include <map>
#include <array>
#include <string>
#include "../generator/Generator.h"
class Gui
{
private:
public:

	const std::string version_number = "0.2-alpha";
	std::string github_url = "https://github.com/stumburs/WallpaperGenerator";

	Texture2D preview_texture{};

	Generator* generator;

	int kWindowWidth;
	int kWindowHeight;

	// Main menu
	Rectangle create_rect;
	Rectangle view_rect;
	Rectangle github_link_rect;

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

	std::array<std::string, 11> flowfield_tooltips = {
		"Final image horizontal resolution",
		"Final image vertical resolution",
		"Lower number = more detail, but worse performance",
		"How accurately particles follow the flowfield",
		"Amount of particles",
		"How quickly particles move",
		"Particle color intensity",
		"Perlin noise octaves. Higher number = worse performance",
		"Noise x-axis multiplier",
		"Noise y-axis multiplier",
		"Noise z-axis multiplier"
	};

	std::array<Rectangle, 11> flowfield_setting_rects = { {
		{ 60 + 50, 40 + 50, 400, 40 },
		{ 60 + 50, 100 + 50, 400, 40 },
		{ 60 + 50, 160 + 50, 400, 40 },
		{ 60 + 50, 280 + 50, 400, 40 },
		{ 60 + 50, 340 + 50, 400, 40 },
		{ 60 + 50, 400 + 50, 400, 40 },
		{ 60 + 50, 520 + 50, 400, 40 },
		{ 60 + 50, 640 + 50, 400, 40 },
		{ 60 + 50, 700 + 50, 400, 40 },
		{ 60 + 50, 760 + 50, 400, 40 },
		{ 60 + 50, 820 + 50, 400, 40 }
	} };

	std::array<Rectangle, 11> flowfield_setting_rects_and_scroll_pos = flowfield_setting_rects;

	std::array<std::string, 11> flowfield_values = {
		"window_width",
		"window_height",
		"scale",
		"flowfield_strength",
		"particle_count",
		"particle_speed",
		"particle_strength",
		"noise_detail",
		"x_mult",
		"y_mult",
		"z_mult",
	};

	int active_menu = Menu::MAIN;
	int active_algorithm = Generator::Generators::NONE;
	int active_blend_mode;

	Gui(int kWindowWidth, int kWindowHeight, Generator *generator);
	void Draw();
	void Update();
	void MainMenuScreen();
	void CreateScreen();
	void FlowfieldScreen();
	void ShapesScreen();

};

