#include "Shapes.h"
#include <random>
#include "../../Functions.h"

Shapes::Shapes()
{
	default_settings =
	{
		{ "Window Width", 1920, 0, std::pair<float, float>{ static_cast<float>(2), static_cast<float>(1920) }, "Final image horizontal resolution", InputType::GUI_TEXT_BOX, "1920", false },
		{ "Window Height", 1080, 0, std::pair<float, float>{ static_cast<float>(2), static_cast<float>(1080) }, "Final image vertical resolution", InputType::GUI_TEXT_BOX, "1080", false },
		{ "Seed", 69420, 0, std::pair<float, float>{static_cast<float>(0), static_cast<float>(UINT32_MAX) }, "Random noise seed", InputType::GUI_TEXT_BOX, "1234", false },
		{ "Shapes Amount", 20, 0, std::pair<float, float>{ static_cast<float>(1), static_cast<float>(100) }, "Amount of shapes to draw", InputType::GUI_TEXT_BOX, "20", false },
		{ "Background Color", 0, 0, std::pair<float, float>{ static_cast<float>(0), static_cast<float>(255255255) }, "Background color", InputType::GUI_COLOR_PICKER, "000000000"},
		{ "Shape Alpha", 255, 0, std::pair<float, float>{ static_cast<float>(1), static_cast<float>(255) }, "How transparent each shape is", InputType::GUI_SLIDER_BAR  },
		{ "Blend Mode", BlendMode::BLEND_ADDITIVE, 0, std::pair<float, float>{ static_cast<float>(0), static_cast<float>(5) }, "Particle and background image blending."}
	};

	user_settings = default_settings;

	Shapes::ApplySettings();
}

std::vector<Generator::Setting>& Shapes::GetUserSettings()
{
	return user_settings;
}

void Shapes::Update()
{
	srand(seed);
	// Draw to texture
	while (shapes_drawn < shapes_amount)
	{
		Color c = { static_cast<unsigned char>(rand() % 256), static_cast<unsigned char>(rand() % 256), static_cast<unsigned char>(rand() % 256), static_cast<unsigned char>(shape_alpha) };
		BeginTextureMode(image);
		BeginBlendMode(blend_mode);
		DrawPoly({ (float)(rand() % window_width), (float)(rand() % window_height) }, static_cast<int>(rand() % 8 + 2), static_cast<float>(rand() % 100 + 100), static_cast<float>(rand() % 100 * 0.1f), c);
		shapes_drawn++;
		EndBlendMode();
		EndTextureMode();
	}
}

void Shapes::ApplySettings()
{
	try
	{
		window_width = std::stoi(user_settings[0].string_value);
		window_height = std::stoi(user_settings[1].string_value);
	}
	catch (const std::exception&) {}
	try
	{
		seed = std::stoi(user_settings[2].string_value);
	}
	catch (const std::exception&) {}
	try
	{
		shapes_amount = std::stoi(user_settings[3].string_value);
	}
	catch (const std::exception&) {}
	background_color = { (unsigned char)std::stoi(user_settings[4].string_value.substr(0, 3)), (unsigned char)std::stoi(user_settings[4].string_value.substr(3, 3)), (unsigned char)std::stoi(user_settings[4].string_value.substr(6, 3)), 255 };
	shape_alpha = static_cast<int>(user_settings[5].value);
	blend_mode = static_cast<int>(user_settings[6].value);
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
