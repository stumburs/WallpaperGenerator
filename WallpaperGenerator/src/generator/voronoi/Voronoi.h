#pragma once
#include "raylib.h"
#include <map>
#include <string>
#include <vector>
#include "../Generator.h"
class Voronoi : public Generator
{
public:

	Color bright_red = { 251, 73, 52, 255 };
	Color bright_green = { 184, 187, 38, 255 };
	Color bright_yellow = { 250, 189, 47, 255 };
	Color bright_blue = { 131, 165, 152, 255 };
	Color bright_purple = { 211, 134, 155, 255 };
	Color bright_aqua = { 142, 192, 124, 255 };
	Color bright_orange = { 254, 128, 24, 255 };

	std::vector<Vector2> seeds;

	std::vector<Color> palette = {
		bright_red,
		bright_green,
		bright_yellow,
		bright_blue,
		bright_purple,
		bright_aqua,
		bright_orange
	};

	size_t palette_count = palette.size();

	std::vector<Setting> default_settings =
	{
		{ "Window Width", 1920, 0, std::pair{ static_cast<float>(2), static_cast<float>(1920) }, "Final image horizontal resolution", InputType::GUI_TEXT_BOX, "1920", false },
		{ "Window Height", 1080, 0, std::pair{ static_cast<float>(2), static_cast<float>(1080) }, "Final image vertical resolution", InputType::GUI_TEXT_BOX, "1080", false },
		{ "Seed", 69420, 0, std::pair{static_cast<float>(0), static_cast<float>(UINT32_MAX)}, "Random noise seed", InputType::GUI_TEXT_BOX, "69420", false},
		{ "Seed Count", 20, 0, std::pair{ static_cast<float>(1), static_cast<float>(100) }, "Amount of seeds for diagram", InputType::GUI_TEXT_BOX, "20", false },
		{ "Render Points", 1, 0, std::pair{ static_cast<float>(0), static_cast<float>(1) }, "Whether to render points", InputType::GUI_CHECK_BOX, "1", false }
	};

	std::vector<Setting> user_settings = default_settings;

	int window_width;
	int window_height;
	int seed;
	int seed_count;
	Color background_color;
	int blend_mode;
	bool render_points;
	bool rendered = false;

	RenderTexture2D image;

public:
	void GenerateRandomSeeds();
	int SqrDist(int x1, int y1, int x2, int y2);
	Voronoi();
	void Init();
	void Update();
	void ApplySettings();
	void ResetSettings();
	std::vector<Setting>& GetUserSettings();
	Texture2D GetImage();
};

