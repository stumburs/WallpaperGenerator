#pragma once
#include "raylib.h"
#include "Particle.h"
#include "../../PerlinNoise.hpp"
#include <map>
#include "../Generator.h"
class Flowfield : public Generator
{
public:
	struct Setting
	{
		std::string name;
		float value;
		int precision;
		std::pair<float, float> range;
		std::string tooltip;
	};

	std::vector<Setting> default_settings =
	{
		{ "Window Width", 1920, 0, { 2, 1920 }, "Final image horizontal resolution" },
		{ "Window Height", 1080, 0, { 2, 1080 }, "Final image vertical resolution" },
		{ "Scale", 20, 0, { 2, 40 }, "Lower number = more detail, but worse performance" },
		{ "Seed", 69420, 0, {0, UINT32_MAX }, "Random noise seed" },
		{ "Flowfield Strength", 0.01f, 3, { 0.001f, 0.1f }, "How accurately particles follow the flowfield" },
		{ "Particle Count", 10000, 0, { 500, 50000 }, "Amount of particles" },
		{ "Particle Speed", 1.0f, 2, { 0, 2.0f }, "How quickly particles move" },
		{ "Particle Strength", 1, 0, { 1, 255 }, "Particle color intensity" },
		{ "Noise Detail", 4, 0, { 1, 16 }, "Perlin noise octaves. Higher number = worse performance" },
		{ "X Multiplier", 0.02f, 2, { 0.01f, 0.3f }, "Noise x-axis multiplier" },
		{ "Y Multiplier", 0.02f, 2, { 0.01f, 0.3f }, "Noise y-axis multiplier" },
		{ "Z Multiplier", 0.02f, 2, { 0.01f, 0.3f }, "Noise z-axis multiplier" },
		{ "Background G", 0, 0, { 0, 255 }, "Background color red" },
		{ "Background B", 0, 0, { 0, 255 }, "Background color green" },
		{ "Background R", 0, 0, { 0, 255 }, "Background color blue" },
		{ "Background A", 255, 0, { 0, 255 }, "Background color transparency" },
		{ "Blend Mode", BlendMode::BLEND_ALPHA_PREMULTIPLY, 0, { 0, 5 }, "Particle and background image blending."}
	};

	std::vector<Setting> user_settings = default_settings;

	int window_width;
	int window_height;
	int scale;
	siv::PerlinNoise::seed_type seed;
	float flowfield_strength;
	int particle_count;
	float particle_speed;
	unsigned char particle_strength;
	int noise_detail;
	float x_mult;
	float y_mult;
	float z_mult;
	unsigned char background_r;
	unsigned char background_g;
	unsigned char background_b;
	unsigned char background_a;
	int blend_mode;

	int render_width;
	int render_height;
	float noise_height;
	float z;


	siv::PerlinNoise perlin;
	std::vector<std::vector<Vector2>> flowfield;
	std::vector<Particle> particles;
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

