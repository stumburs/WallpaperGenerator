#pragma once
#include "raylib.h"
#include "Particle.h"
#include <map>
#include "../../PerlinNoise.hpp"
#include "../Generator.h"
class Flowfield : public Generator
{
public:

	std::vector<Setting> default_settings =
	{
		{ "Window Width", 1920, 0, { 2, 1920 }, "Final image horizontal resolution", InputType::GUI_TEXT_BOX, "1920", false },
		{ "Window Height", 1080, 0, { 2, 1080 }, "Final image vertical resolution", InputType::GUI_TEXT_BOX, "1080", false },
		{ "Seed", 69420, 0, {0, UINT32_MAX }, "Random noise seed", InputType::GUI_TEXT_BOX, "1234", false },
		{ "Scale", 20, 0, { 2, 40 }, "Lower number = more detail, but worse performance", InputType::GUI_SLIDER_BAR },
		{ "Flowfield Strength", 0.01f, 3, { 0.001f, 0.1f }, "How accurately particles follow the flowfield", InputType::GUI_SLIDER_BAR },
		{ "Particle Count", 10000, 0, { 500, 50000 }, "Amount of particles", InputType::GUI_SLIDER_BAR },
		{ "Particle Speed", 1.0f, 2, { 0, 2.0f }, "How quickly particles move", InputType::GUI_SLIDER_BAR },
		{ "Particle Strength", 3, 0, { 3, 255 }, "Particle color intensity", InputType::GUI_SLIDER_BAR },
		{ "Noise Detail", 4, 0, { 1, 16 }, "Perlin noise octaves. Higher number = worse performance", InputType::GUI_SLIDER_BAR },
		{ "X Multiplier", 0.02f, 2, { 0.01f, 0.3f }, "Noise x-axis multiplier", InputType::GUI_SLIDER_BAR },
		{ "Y Multiplier", 0.02f, 2, { 0.01f, 0.3f }, "Noise y-axis multiplier", InputType::GUI_SLIDER_BAR },
		{ "Z Multiplier", 0.02f, 2, { 0.01f, 0.3f }, "Noise z-axis multiplier", InputType::GUI_SLIDER_BAR },
		{ "Background Color", 0, 0, { 0, 255255255 }, "Background color", InputType::GUI_COLOR_PICKER, "000000000"},
		{ "Background A", 255, 0, { 0, 255 }, "Background color transparency", InputType::GUI_SLIDER_BAR },
		{ "Top", 0, 0, { 0, 255255255 }, "Left particle color", InputType::GUI_COLOR_PICKER, "255000000"}, // Actually left
		{ "Bottom", 0, 0, { 0, 255255255 }, "Right particle color", InputType::GUI_COLOR_PICKER, "000255000"}, // Actually right
		{ "Left", 0, 0, { 0, 255255255 }, "Bottom particle color", InputType::GUI_COLOR_PICKER, "000000255"}, // Actually bottom
		{ "Right", 0, 0, { 0, 255255255 }, "Top particle color", InputType::GUI_COLOR_PICKER, "255000255"}, // Actually top
		{ "Blend Mode", BlendMode::BLEND_ALPHA_PREMULTIPLY, 0, { 0, 5 }, "Particle and background image blending.", InputType::GUI_SLIDER_BAR}
	};

	std::vector<Setting> user_settings = default_settings;

	int window_width;
	int window_height;
	siv::PerlinNoise::seed_type seed;
	int scale;
	float flowfield_strength;
	int particle_count;
	float particle_speed;
	unsigned char particle_strength;
	int noise_detail;
	float x_mult;
	float y_mult;
	float z_mult;
	Color background_color;
	unsigned char background_a;
	Color top = { 0 };
	Color bottom = { 0 };
	Color left = { 0 };
	Color right = { 0 };
	int blend_mode;

	int render_width;
	int render_height;
	float noise_height;
	float z;


	siv::PerlinNoise perlin;
	std::vector<std::vector<Vector2>> flowfield;
	std::vector<Particle> particles;
	std::vector<std::vector<Color>> color_vector;
	RenderTexture2D image;
	RenderTexture2D return_image;

public:
	Flowfield();
	void Init();
	void Update();
	void ApplySettings();
	void ResetSettings();
	Texture2D GetImage();
};

