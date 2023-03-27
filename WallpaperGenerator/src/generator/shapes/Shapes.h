#pragma once
#include "raylib.h"
#include <map>
#include <string>
#include <vector>
#include "../Generator.h"
class Shapes : public Generator
{
private:

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
	void Update();
	void ApplySettings();
	void ResetSettings();
	std::vector<Setting>& GetUserSettings();
	Texture2D GetImage();
};

