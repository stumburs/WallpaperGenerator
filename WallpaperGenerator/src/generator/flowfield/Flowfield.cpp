#include "Flowfield.h"
#include "../../PerlinNoise.hpp"
#include "../../Functions.h"
#include <string>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "../../json/JSONReader.h"
Flowfield::Flowfield()
{
	default_settings = JSONReader::LoadSettingsFromJson(std::string("funny_json_path.json"));
	//default_settings =
	//{
	//	{ "Window Width", 1920, 0, std::pair<float, float>{ static_cast<float>(2), static_cast<float>(1920) }, "Final image horizontal resolution", InputType::GUI_TEXT_BOX, "1920", false },
	//	{ "Window Height", 1080, 0, std::pair<float, float>{ static_cast<float>(2), static_cast<float>(1080) }, "Final image vertical resolution", InputType::GUI_TEXT_BOX, "1080", false },
	//	{ "Seed", 69420, 0, std::pair<float, float>{static_cast<float>(0), static_cast<float>(UINT32_MAX) }, "Random noise seed", InputType::GUI_TEXT_BOX, "1234", false },
	//	{ "Scale", 20, 0, std::pair<float, float>{ static_cast<float>(2), static_cast<float>(40) }, "Lower number = more detail, but worse performance", InputType::GUI_SLIDER_BAR },
	//	{ "Flowfield Strength", 0.01f, 3, std::pair<float, float>{ 0.001f, 0.1f }, "How accurately particles follow the flowfield", InputType::GUI_SLIDER_BAR },
	//	{ "Particle Count", 10000, 0, std::pair<float, float>{ static_cast<float>(500), static_cast<float>(50000) }, "Amount of particles", InputType::GUI_SLIDER_BAR },
	//	{ "Particle Speed", 1.0f, 2, std::pair<float, float>{ static_cast<float>(0), 2.0f }, "How quickly particles move", InputType::GUI_SLIDER_BAR },
	//	{ "Particle Strength", 3, 0, std::pair<float, float>{ static_cast<float>(3), static_cast<float>(255) }, "Particle color intensity", InputType::GUI_SLIDER_BAR },
	//	{ "Noise Detail", 4, 0, std::pair<float, float>{ static_cast<float>(1), static_cast<float>(16) }, "Perlin noise octaves. Higher number = worse performance", InputType::GUI_SLIDER_BAR },
	//	{ "X Multiplier", 0.02f, 2, std::pair{ 0.01f, 0.3f }, "Noise x-axis multiplier", InputType::GUI_SLIDER_BAR },
	//	{ "Y Multiplier", 0.02f, 2, std::pair<float, float>{ 0.01f, 0.3f }, "Noise y-axis multiplier", InputType::GUI_SLIDER_BAR },
	//	{ "Z Multiplier", 0.02f, 2, std::pair<float, float>{ 0.01f, 0.3f }, "Noise z-axis multiplier", InputType::GUI_SLIDER_BAR },
	//	{ "Background Color", 0, 0, std::pair<float, float>{ static_cast<float>(0), static_cast<float>(255255255) }, "Background color", InputType::GUI_COLOR_PICKER, "000000000"},
	//	{ "Background A", 255, 0, std::pair<float, float>{ static_cast<float>(0), static_cast<float>(255) }, "Background color transparency", InputType::GUI_SLIDER_BAR },
	//	{ "Top", 0, 0, std::pair<float, float>{ static_cast<float>(0), static_cast<float>(255255255) }, "Left particle color", InputType::GUI_COLOR_PICKER, "255000000"}, // Actually left
	//	{ "Bottom", 0, 0, std::pair<float, float>{ static_cast<float>(0), static_cast<float>(255255255) }, "Right particle color", InputType::GUI_COLOR_PICKER, "000255000"}, // Actually right
	//	{ "Left", 0, 0, std::pair<float, float>{ static_cast<float>(0), static_cast<float>(255255255) }, "Bottom particle color", InputType::GUI_COLOR_PICKER, "000000255"}, // Actually bottom
	//	{ "Right", 0, 0, std::pair<float, float>{ static_cast<float>(0), static_cast<float>(255255255) }, "Top particle color", InputType::GUI_COLOR_PICKER, "255000255"}, // Actually top
	//	{ "Blend Mode", BlendMode::BLEND_ALPHA_PREMULTIPLY, 0, std::pair<float, float>{ static_cast<float>(0), static_cast<float>(5) }, "Particle and background image blending.", InputType::GUI_SLIDER_BAR}
	//};

	user_settings = default_settings;

	Flowfield::InitializeDefaultVariablesFromSettings();
	Flowfield::ApplySettings();
}

void Flowfield::InitializeDefaultVariablesFromSettings()
{
	window_width = std::get<float>(default_settings.second["Window Width"].value);
	window_height = std::get<float>(default_settings.second["Window Height"].value);
	seed = std::get<float>(default_settings.second["Seed"].value);
	scale = std::get<float>(default_settings.second["Scale"].value);
	flowfield_strength = std::get<float>(default_settings.second["Flowfield Strength"].value);
	particle_count = std::get<float>(default_settings.second["Particle Count"].value);
	particle_speed = std::get<float>(default_settings.second["Particle Speed"].value);
	particle_strength = std::get<float>(default_settings.second["Particle Strength"].value);
	noise_detail = std::get<float>(default_settings.second["Noise Detail"].value);
	x_mult = std::get<float>(default_settings.second["X Multiplier"].value);
	y_mult = std::get<float>(default_settings.second["Y Multiplier"].value);
	z_mult = std::get<float>(default_settings.second["Z Multiplier"].value);
	background_color = std::get<Color>(default_settings.second["Background Color"].value);
	background_a = std::get<float>(default_settings.second["Background A"].value);
	top = std::get<Color>(default_settings.second["Top"].value);
	bottom = std::get<Color>(default_settings.second["Bottom"].value);
	left = std::get<Color>(default_settings.second["Left"].value);
	right = std::get<Color>(default_settings.second["Right"].value);
}

std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>>& Flowfield::GetUserSettings()
{
	return user_settings;
}

void Flowfield::Update()
{
	// Flowfield
	for (int x = 0; x < flowfield.size(); x++)
	{
		for (int y = 0; y < flowfield[x].size(); y++)
		{
			// Calculate each vector
			double angle = Map(perlin.octave3D_01((x * x_mult), (y * y_mult), (z * z_mult), noise_detail), 0, 1, 0, 359);
			Vector2 vec = Vec2FromAngle((float)angle);
			vec = SetMagnitude(vec, flowfield_strength);
			flowfield[x][y] = vec;
		}
	}

	// Update particles
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].Update(flowfield, particle_speed, window_width, window_height, scale);
	}

	// Draw to texture
	BeginTextureMode(image);
	{
		// Draw Particles
		for (int i = 0; i < particles.size(); i++)
		{
			Color c = particles[i].GetColorAtPos(color_vector);
			particles[i].DrawPixel(c);
		}
	}
	EndTextureMode();

	// Move through noise
	z += 0.06f;
}

void Flowfield::ApplySettings()
{
	perlin.reseed(seed);
	render_width = window_width / scale + 1;
	render_height = window_height / scale + 1;
	z = 0;

	color_vector.clear();
	// Init color vector
	for (int x = 0; x < window_height; x++)
	{
		std::vector<Color> y_vec;
		for (int y = 0; y < window_width; y++)
		{

			Color top_to_bottom = { 0 };
			top_to_bottom.r = static_cast<unsigned char>(SimpleLerp((float)top.r, (float)bottom.r, static_cast<float>(Map(x, 0, window_height - 1, 0, 1))));
			top_to_bottom.g = static_cast<unsigned char>(SimpleLerp((float)top.g, (float)bottom.g, static_cast<float>(Map(x, 0, window_height - 1, 0, 1))));
			top_to_bottom.b = static_cast<unsigned char>(SimpleLerp((float)top.b, (float)bottom.b, static_cast<float>(Map(x, 0, window_height - 1, 0, 1))));
			top_to_bottom.a = 255;

			Color left_to_right = { 0 };
			left_to_right.r = static_cast<unsigned char>(SimpleLerp((float)left.r, (float)right.r, static_cast<float>(Map(y, 0, window_width - 1, 0, 1))));
			left_to_right.g = static_cast<unsigned char>(SimpleLerp((float)left.g, (float)right.g, static_cast<float>(Map(y, 0, window_width - 1, 0, 1))));
			left_to_right.b = static_cast<unsigned char>(SimpleLerp((float)left.b, (float)right.b, static_cast<float>(Map(y, 0, window_width - 1, 0, 1))));
			top_to_bottom.a = 255;

			Color result = { 0 };
			result.r = static_cast<unsigned char>(SimpleClamp(((int)left_to_right.r + (int)top_to_bottom.r) / 2, 0, 255));
			result.g = static_cast<unsigned char>(SimpleClamp(((int)left_to_right.g + (int)top_to_bottom.g) / 2, 0, 255));
			result.b = static_cast<unsigned char>(SimpleClamp(((int)left_to_right.b + (int)top_to_bottom.b) / 2, 0, 255));
			result.a = particle_strength;
			y_vec.push_back({ result });
		}
		color_vector.push_back(y_vec);
	}

	// Particles
	particles.clear();
	for (int i = 0; i < particle_count; i++)
	{
		Particle p{};
		p.pos = { (float)GetRandomValue(0, window_width), (float)GetRandomValue(0, window_height) };
		particles.push_back(p);
	}

	// Flowfield
	flowfield.clear();
	for (int x = 0; x < render_width; x++)
	{
		flowfield.push_back(std::vector<Vector2>());
		for (int y = 0; y < render_height; y++)
		{
			// Calculate each vector
			float angle = (float)Map(perlin.octave3D_01((x * x_mult), (y * y_mult), (z * z_mult), noise_detail), 0, 1, 0, 359);
			Vector2 vec = Vec2FromAngle(angle);
			vec = SetMagnitude(vec, 1);
			flowfield[x].push_back(vec);
		}
	}

	UnloadRenderTexture(image);
	UnloadRenderTexture(return_image);
	image = LoadRenderTexture(window_width, window_height);
	return_image = LoadRenderTexture(window_width, window_height);
	BeginTextureMode(image);
	ClearBackground(BLANK);
	EndTextureMode();
}

// Init / Reset image
void Flowfield::ResetSettings()
{
	user_settings = default_settings;
}

Texture2D Flowfield::GetImage()
{
	BeginTextureMode(return_image);
	ClearBackground({ background_color.r, background_color.g, background_color.b, background_a });
	BeginBlendMode(BlendMode::BLEND_ALPHA_PREMULTIPLY);
	DrawTexturePro(image.texture, { 0, 0, (float)image.texture.width, (float)image.texture.height }, { 0, 0, (float)return_image.texture.width, (float)return_image.texture.height }, { 0, 0 }, 0.0f, WHITE);
	EndBlendMode();
	EndTextureMode();
	return return_image.texture;
}