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
	/*default_settings =
	{
		{ "Window Width", 1920, 0, std::pair<float, float>{ static_cast<float>(2), static_cast<float>(1920) }, "Final image horizontal resolution", InputType::GUI_TEXT_BOX, "1920", false },
		{ "Window Height", 1080, 0, std::pair<float, float>{ static_cast<float>(2), static_cast<float>(1080) }, "Final image vertical resolution", InputType::GUI_TEXT_BOX, "1080", false },
		{ "Seed", 69420, 0, std::pair<float, float>{static_cast<float>(0), static_cast<float>(UINT32_MAX) }, "Random noise seed", InputType::GUI_TEXT_BOX, "1234", false },
		{ "Shapes Amount", 20, 0, std::pair<float, float>{ static_cast<float>(1), static_cast<float>(100) }, "Amount of shapes to draw", InputType::GUI_TEXT_BOX, "20", false },
		{ "Background Color", 0, 0, std::pair<float, float>{ static_cast<float>(0), static_cast<float>(255255255) }, "Background color", InputType::GUI_COLOR_PICKER, "000000000"},
		{ "Shape Alpha", 255, 0, std::pair<float, float>{ static_cast<float>(1), static_cast<float>(255) }, "How transparent each shape is", InputType::GUI_SLIDER_BAR  },
		{ "Blend Mode", BlendMode::BLEND_ADDITIVE, 0, std::pair<float, float>{ static_cast<float>(0), static_cast<float>(5) }, "Particle and background image blending."}
	};*/

	user_settings = default_settings;

	Shapes::ApplySettings();
}

std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>>& Shapes::GetUserSettings()
{
	return user_settings;
}

void Shapes::InitializeDefaultVariablesFromSettings()
{
	window_width = std::get<float>(default_settings.second["Window Width"].value);
	window_height = std::get<float>(default_settings.second["Window Height"].value);
	seed = std::get<float>(default_settings.second["Seed"].value);
	shapes_amount = std::get<float>(default_settings.second["Shapes Amount"].value);
	background_color = std::get<Color>(default_settings.second["Background Color"].value);
	shape_alpha = std::get<float>(default_settings.second["Shape Alpha"].value);
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
