#pragma once
#include "raylib.h"
#include <map>
#include <string>
#include <vector>
#include "../Generator.h"
class Shapes : public Generator
{
public:

	std::vector<Setting> default_settings =
	{
		{ "Window Width", 1920, 0, std::pair<float, float>{ static_cast<float>(2), static_cast<float>(1920) }, "Final image horizontal resolution", InputType::GUI_TEXT_BOX, "1920", false },
		{ "Window Height", 1080, 0, std::pair<float, float>{ static_cast<float>(2), static_cast<float>(1080) }, "Final image vertical resolution", InputType::GUI_TEXT_BOX, "1080", false },
		{ "Seed", 69420, 0, std::pair<float, float>{static_cast<float>(0), static_cast<float>(UINT32_MAX) }, "Random noise seed", InputType::GUI_TEXT_BOX, "1234", false },
		{ "Shapes Amount", 20, 0, std::pair<float, float>{ static_cast<float>(1), static_cast<float>(100) }, "Amount of shapes to draw", InputType::GUI_TEXT_BOX, "20", false },
		{ "Background Color", 0, 0, std::pair<float, float>{ static_cast<float>(0), static_cast<float>(255255255) }, "Background color", InputType::GUI_COLOR_PICKER, "000000000"},
		{ "Shape Alpha", 255, 0, std::pair<float, float>{ static_cast<float>(1), static_cast<float>(255) }, "How transparent each shape is", InputType::GUI_SLIDER_BAR  },
		{ "Blend Mode", BlendMode::BLEND_ADDITIVE, 0, std::pair<float, float>{ static_cast<float>(0), static_cast<float>(5) }, "Particle and background image blending."}
	};

	std::vector<Setting> user_settings = default_settings;

	int window_width;
	int window_height;
	int seed;
	int shapes_amount;
	Color background_color;
	int shape_alpha;
	int blend_mode;

	int shapes_drawn = 0;
	RenderTexture2D image;

public:
	Shapes();
	void Init();
	void Update();
	void ApplySettings();
	void ResetSettings();
	std::vector<Setting>& GetUserSettings();
	Texture2D GetImage();
};

