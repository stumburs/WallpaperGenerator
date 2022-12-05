#pragma once
#include "raylib.h"
#include "Particle.h"
#include "../../PerlinNoise.hpp"
#include <map>
class Flowfield
{
public:

	int window_width;
	int window_height;

	std::map<std::string, float> default_values = {
		{ "window_width", 1920 },
		{ "window_height", 1080 },
		{ "scale", 20 },
		{ "seed", 69420u},
		{ "flowfield_strength", 0.01f },
		{ "particle_count", 10000},
		{ "particle_speed", 1.0f },
		{ "particle_size", 1.0f },
		{ "particle_strength", 1u },
		{ "noise_height", 0.0f },
		{ "noise_detail", 4},
		{ "x_mult", 0.02f},
		{ "y_mult", 0.02f},
		{ "z_mult", 0.02f},
		{ "z", 0 },
		{ "active_blend_mode", BlendMode::BLEND_ALPHA_PREMULTIPLY }
	};

	std::map<std::string, float> user_values = default_values;

	int scale;
	int render_width;
	int render_height;

	siv::PerlinNoise::seed_type seed;

	float flowfield_strength;
	int particle_count;
	float particle_speed;
	float particle_size;
	unsigned char particle_strength;

	float noise_height;
	int noise_detail;
	float x_mult;
	float y_mult;
	float z_mult;
	float z;
	int active_blend_mode;


	siv::PerlinNoise perlin;
	std::vector<std::vector<Vector2>> flowfield;
	std::vector<Particle> particles;
	RenderTexture2D image;
	RenderTexture2D return_image;

public:
	Flowfield();
	void InitValues();
	void Update();
	Texture2D GetImage();
	void ResetImage();
	void Reset();
	void InitParticles();
	void InitFlowfield();
	std::map<std::string, float> GetUserValues();
};

