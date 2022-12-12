#pragma once
#include "raylib.h"
#include <map>
#include <string>
class Shapes
{
public:

	std::map<std::string, float> default_values = {
		{ "window_width", 1920 },
		{ "window_height", 1080 },
		{ "shapes_to_draw", 20 },
		{ "seed", 69420u },
		{ "active_blend_mode", BlendMode::BLEND_ALPHA_PREMULTIPLY }
	};

	std::map<std::string, float> user_values = default_values;
	int window_width;
	int window_height;
	int shapes_drawn = 0;
	int shapes_to_draw;
	int active_blend_mode;

	RenderTexture2D image;

public:
	Shapes();
	void Update();
	void Reset();
	Texture2D GetImage();
};

