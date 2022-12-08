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
	const std::string github_url = "https://github.com/stumburs/WallpaperGenerator";

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

	std::map<std::string, float> default_values = {
		{ "window_width", 1920 },
		{ "window_height", 1080 },
		{ "scale", 20 },
		{ "flowfield_strength", 0.01f },
		{ "particle_count", 10000},
		{ "particle_speed", 1.0f },
		{ "particle_strength", 1u },
		{ "noise_height", 0.0f },
		{ "noise_detail", 4},
		{ "x_mult", 0.02f},
		{ "y_mult", 0.02f},
		{ "z_mult", 0.02f},
	};

	std::map<std::string, float> flowfield_settings = {
		{ "window_width", (int)default_values["window_width"] },
		{ "window_height", (int)default_values["window_height"] },
		{ "scale", (int)default_values["scale"] },
		{ "flowfield_strength", default_values["flowfield_strength"] },
		{ "particle_count", (int)default_values["particle_count"] },
		{ "particle_speed", default_values["particle_speed"] },
		{ "particle_strength", (unsigned char)default_values["particle_strength"] },
		{ "noise_detail", (int)default_values["noise_detail"] },
		{ "x_mult", default_values["x_mult"] },
		{ "y_mult", default_values["y_mult"] },
		{ "z_mult", default_values["z_mult"] }
	};

	std::array<std::string, 11> flowfield_settings_keys = {
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
		"z_mult"
	};

	std::map<std::string, int> flowfield_settings_precision = {
		{"window_width", 0},
		{"window_height", 0},
		{"scale", 0},
		{"flowfield_strength", 3},
		{"particle_count", 0},
		{"particle_speed", 2},
		{"particle_strength", 0},
		{"noise_detail", 0},
		{"x_mult", 2},
		{"y_mult", 2},
		{"z_mult", 2}
	};

	std::map<std::string, std::pair<float, float>> flowfield_settings_ranges = {
		{ "window_width", { 2, 1920 } },
		{ "window_height", { 2, 1080 } },
		{ "scale", { 1, 40 } },
		{ "flowfield_strength", { 0.001f, 0.1f } },
		{ "particle_count", { 500, 50000 } },
		{ "particle_speed", { 0, 2.0f } },
		{ "particle_strength", { 1, 16 } },
		{ "noise_detail", { 1, 16 } },
		{ "x_mult", { 0.01f, 0.3f } },
		{ "y_mult", { 0.01f, 0.3f } },
		{ "z_mult", { 0.01f, 0.3f } }
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

	int active_menu = Menu::MAIN;
	int active_algorithm = Generator::Generators::NONE;
	int active_blend_mode;

	std::array<std::string, 7> blend_mode_names = {
		"BLEND_ALPHA",
		"BLEND_ADDITIVE",
		"BLEND_MULTIPLIED",
		"BLEND_ADD_COLORS",
		"BLEND_SUBTRACT_COLORS",
		"BLEND_ALPHA_PREMULTIPLY",
		"BLEND_CUSTOM"
	};

	Gui(int kWindowWidth, int kWindowHeight, Generator *generator);
	void Draw();
	void Update();
	void MainMenuScreen();
	void CreateScreen();
	void FlowfieldScreen();
	void ShapesScreen();

};