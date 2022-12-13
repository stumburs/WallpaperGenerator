#pragma once
#include "raylib.h"
#include <map>
#include <string>
#include <vector>
#include "../Generator.h"
class Shapes : public Generator
{
public:

	struct Setting
	{
		std::string name;
		float value;
		int precision;
		std::pair<float, float> range;
		std::string tooltip;
	};

	std::vector<Setting> default_settings =
	{
		{ "Window Width", 1920, 0, { 2, 1920 }, "Final image horizontal resolution" },
		{ "Window Height", 1080, 0, { 2, 1080 }, "Final image vertical resolution" },
		{ "Shapes Amount", 20, 0, { 1, 100 }, "Amount of shapes to draw" },
		{ "Shape Alpha", 255, 0, { 1, 255 }, "How transparent each shape is" },
		{ "Seed", 69420, 0, {0, UINT32_MAX }, "Random noise seed" },
		{ "Blend Mode", BlendMode::BLEND_ADDITIVE, 0, { 0, 5 }, "Particle and background image blending."}
	};

	std::vector<Setting> user_settings = default_settings;

	int window_width;
	int window_height;
	int shapes_amount;
	int shape_alpha;
	int seed;
	int blend_mode;

	int shapes_drawn = 0;
	RenderTexture2D image;

public:
	Shapes();
	void Init();
	void Update();
	void ApplySettings();
	void ResetSettings();
	Texture2D GetImage();
};

