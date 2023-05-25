#pragma once
#include "raylib.h"
#include "Particle.h"
#include <map>
#include "../../PerlinNoise.hpp"
#include "../Generator.h"
class Flowfield : public Generator
{
private:

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
	void Update();
	void ApplySettings();
	void ResetSettings();
	std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>>& GetUserSettings();
	void InitializeDefaultVariablesFromSettings();
	void SaveSettingsToJson();
	Texture2D GetImage();
};

