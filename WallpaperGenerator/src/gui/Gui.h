#pragma once
#include <raylib.h>
#include <map>
#include <array>
#include <string>
#include "../generator/Generators.h"
#include <cmath>
#include "../libraries/raygui-3.2/raygui.h"
class Gui
{
private:

	Rectangle ScaleRect(Rectangle rect, float scale_x, float scale_y)
	{
		float rect_x = round(rect.x * scale_x);
		float rect_y = round(rect.y * scale_y);
		float rect_width = round(rect.width * scale_x);
		float rect_height = round(rect.height * scale_y);
		return { rect_x, rect_y, rect_width, rect_height };
	}

	const std::string version_number = "0.7-alpha";
	const std::string github_url = "https://github.com/stumburs/WallpaperGenerator";

	Texture2D preview_texture{};

	Generators generators;

	int window_width;
	int window_height;

	float scale_x = GetScreenWidth() / 1600;
	float scale_y = GetScreenHeight() / 900;

	const int orig_title_font_size = 60;
	int scaled_title_font_size = round(orig_title_font_size * std::min(scale_x, scale_y));
	const int orig_version_font_size = 20;
	int scaled_version_font_size = round(orig_version_font_size * std::min(scale_x, scale_y));

	const int default_text_size = 20;
	int scaled_text_size = round(default_text_size * std::min(scale_x, scale_y));

	// Main menu
	Rectangle orig_create_rect;
	Rectangle create_rect;

	Rectangle orig_view_rect;
	Rectangle view_rect;

	Rectangle orig_github_link_rect;
	Rectangle github_link_rect;

	Rectangle orig_back_rect_center;
	Rectangle back_rect_center;

	Rectangle orig_back_rect_corner;
	Rectangle back_rect_corner;

	// Create menu
	Texture flowfield_demo_image;
	Texture voronoi_demo_image;
	Texture shapes_demo_image;

	Rectangle orig_flowfield_rect;
	Rectangle flowfield_rect;

	Rectangle orig_shapes_rect;
	Rectangle shapes_rect;

	Rectangle orig_voronoi_rect;
	Rectangle voronoi_rect;

	// Generate
	Rectangle orig_preview_rect;
	Rectangle preview_rect;

	Rectangle orig_save_image;
	Rectangle save_image;

	Rectangle orig_update_settings;
	Rectangle update_settings;

	Rectangle orig_reset_settings;
	Rectangle reset_settings;

	Vector2 orig_scroll_pos;
	Vector2 scroll_pos;

	Rectangle orig_scissor_bounds = { 50, 45, 655, 800 };
	Rectangle scissor_bounds = { 50, 45, 655, 800 };
	Rectangle orig_scissor_content = { 50, 50, 635, 2000 };
	Rectangle scissor_content = { 50, 50, 635, 2000 };
	Rectangle orig_scissor_area = GuiScrollPanel(scissor_bounds, "Settings", scissor_content, &scroll_pos);
	Rectangle scissor_area = orig_scissor_area;

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

	Rectangle orig_first_setting_rect = { 110, 90, 400, 40 };
	Rectangle first_setting_rect = orig_first_setting_rect;


	int active_menu = Menu::MAIN;
	int active_generator = Gui::ActiveGenerator::NONE;

public:
	Gui(int window_width, int window_height);
	void Draw();
	void Update();
	void MainMenuScreen();
	void CreateScreen();
	void ViewScreen();
	void GeneratorScreen(Generators& generators);

};
