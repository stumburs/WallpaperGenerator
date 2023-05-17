#include "Shapes.h"
#include <random>
#include "../../Functions.h"
#include "../../json/JSONReader.h"

Shapes::Shapes()
{
	std::cout << "Shapes\n";
	std::cout << "App directory: " << GetApplicationDirectory() << '\n';
	std::cout << "Attempting to load shapes.json\n";
	default_settings = JSONReader::LoadSettingsFromJson(std::string("cfg/shapes.json"));
	user_settings = default_settings;
	Shapes::InitializeDefaultVariablesFromSettings();
	Shapes::ApplySettings();
}

std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>>& Shapes::GetUserSettings()
{
	return user_settings;
}

void Shapes::InitializeDefaultVariablesFromSettings()
{
	window_width = std::get<float>(user_settings.second["Window Width"].value);
	window_height = std::get<float>(user_settings.second["Window Height"].value);
	seed = std::get<float>(user_settings.second["Seed"].value);
	shapes_amount = std::get<float>(user_settings.second["Shapes Amount"].value);
	background_color = std::get<Color>(user_settings.second["Background Color"].value);
	shape_alpha = std::get<float>(user_settings.second["Shape Alpha"].value);
}

void Shapes::Update()
{
	srand(seed);
	// Draw to texture
	while (shapes_drawn < shapes_amount)
	{
		Color c = { static_cast<unsigned char>(rand() % 256), static_cast<unsigned char>(rand() % 256), static_cast<unsigned char>(rand() % 256), static_cast<unsigned char>(shape_alpha) };
		BeginTextureMode(image);
		BeginBlendMode(BlendMode::BLEND_ALPHA_PREMULTIPLY);
		DrawPoly({ (float)(rand() % window_width), (float)(rand() % window_height) }, static_cast<int>(rand() % 8 + 2), static_cast<float>(rand() % 100 + 100), static_cast<float>(rand() % 100 * 0.1f), c);
		shapes_drawn++;
		EndBlendMode();
		EndTextureMode();
	}
}

void Shapes::ApplySettings()
{
	InitializeDefaultVariablesFromSettings();
	shapes_drawn = 0;

	// Init texture
	UnloadRenderTexture(image);
	image = LoadRenderTexture(window_width, window_height);
	BeginTextureMode(image);
	ClearBackground(background_color);
	EndTextureMode();
}

void Shapes::ResetSettings()
{
	user_settings = default_settings;
}

Texture2D Shapes::GetImage()
{
	return image.texture;
}
