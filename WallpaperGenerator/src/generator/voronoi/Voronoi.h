#pragma once
#include "raylib.h"
#include <map>
#include <string>
#include <vector>
#include "../Generator.h"
class Voronoi : public Generator
{
private:

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
		bright_orange,
	};

	size_t palette_count = palette.size();

	int window_width;
	int window_height;
	int seed;
	int seed_count;
	Color background_color;
	int blend_mode;
	bool render_points;
	bool rendered = false;

	RenderTexture2D image;

	void GenerateRandomSeeds();
	int SqrDist(int x1, int y1, int x2, int y2);

public:
	Voronoi();
	void Update();
	void ApplySettings();
	void ResetSettings();
	std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>>& GetUserSettings();
	void InitializeDefaultVariablesFromSettings();
	void SaveSettingsToJson();
	Texture2D GetImage();
};

