#pragma once
#include <map>
#include <string>
#include <vector>
#include "../Generator.h"

class FractalTree : public Generator
{
private:
	int window_width;
	int window_height;

	float fork_angle = PI / 8;

	int recursion_depth = 15;

	float thick = 10;
	float thick_change = 0.8;

	float len = 150;
	float len_change = 0.85;

	float left_change = 1;
	float right_change = 0.5;

	Color background_color;
	Color color = { 255, 255, 255, 255 };

	bool rendered = false;
	RenderTexture2D image;

public:
	FractalTree();
	void Update();
	void ApplySettings();
	void ResetSettings();
	std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>>& GetUserSettings();
	void InitializeDefaultVariablesFromSettings();
	void SaveSettingsToJson();
	Texture2D GetImage();

private:
	void DrawTree(float start_x, float start_y, float angle, float thick, int depth, float len);
};