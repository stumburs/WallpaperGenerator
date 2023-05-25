#include "FractalTree.h"
#include <random>
#include "../../Functions.h"
#include "../../json/JSONReader.h"
#include "../../json/JSONWriter.h"

FractalTree::FractalTree()
{
	std::cout << "Fractal Tree\n";
	std::cout << "App directory: " << GetApplicationDirectory() << '\n';
	std::cout << "Attempting to load fractal_tree.json\n";
	default_settings = JSONReader::LoadSettingsFromJson(std::string("cfg\\fractal_tree.json"));
	if (FileExists("cfg\\autosave_fractal_tree.json"))
	{
		std::cout << "Loading autosaved settings for Fractal Tree\n";
		user_settings = JSONReader::LoadSettingsFromJson(std::string("cfg\\autosave_fractal_tree.json"));
	}
	else
		user_settings = default_settings;
	FractalTree::InitializeDefaultVariablesFromSettings();
	FractalTree::ApplySettings();
}

std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>>& FractalTree::GetUserSettings()
{
	return user_settings;
}

void FractalTree::InitializeDefaultVariablesFromSettings()
{
	window_width = std::get<float>(user_settings.second["Window Width"].value);
	window_height = std::get<float>(user_settings.second["Window Height"].value);
	fork_angle = std::get<float>(user_settings.second["Fork Angle"].value);
	recursion_depth = std::get<float>(user_settings.second["Recursion Depth"].value);
	thick = std::get<float>(user_settings.second["Line Thickness"].value);
	thick_change = std::get<float>(user_settings.second["Line Thickness Change"].value);
	len = std::get<float>(user_settings.second["Line Length"].value);
	len_change = std::get<float>(user_settings.second["Line Length Change"].value);
	left_change = std::get<float>(user_settings.second["Left Angle Change"].value);
	right_change = std::get<float>(user_settings.second["Right Angle Change"].value);
	color = std::get<Color>(user_settings.second["Line Color"].value);
	background_color = std::get<Color>(user_settings.second["Background Color"].value);
}

void FractalTree::SaveSettingsToJson()
{
	JSONWriter::WriteSettingsToJson(user_settings, "cfg\\autosave_fractal_tree.json");
}

void FractalTree::Update()
{
	if (!rendered)
	{
		BeginTextureMode(image);
		ClearBackground(background_color);
		DrawTree(window_width / static_cast<float>(2), window_height, PI / 2, thick, recursion_depth, len);
		EndTextureMode();
		rendered = true;
	}
}

void FractalTree::DrawTree(float start_x, float start_y, float angle, float thick, int depth, float len) {
	if (depth > 0) {
		thick *= thick_change;
		len *= len_change;
		float end_x = start_x + cos(angle) * len;
		float end_y = start_y - sin(angle) * len;

		DrawLineEx({ start_x, start_y }, { end_x, end_y }, thick, color);
		DrawTree(end_x, end_y, angle - fork_angle * right_change, thick, depth - 1, len);
		DrawTree(end_x, end_y, angle + fork_angle * left_change, thick, depth - 1, len);
	}
}

void FractalTree::ApplySettings()
{
	InitializeDefaultVariablesFromSettings();
	rendered = false;

	// Init texture
	UnloadRenderTexture(image);
	image = LoadRenderTexture(window_width, window_height);
	BeginTextureMode(image);
	ClearBackground(background_color);
	EndTextureMode();
}

void FractalTree::ResetSettings()
{
	user_settings = default_settings;
}

Texture2D FractalTree::GetImage()
{
	return image.texture;
}
