#pragma once
#include "raylib.h"
#include "Particle.h"
#include "../../PerlinNoise.hpp"
#include <map>
class Flowfield
{
public:
	int window_width = 1600;
	int window_height = 900;

	int scale = 20;
	int render_width;
	int render_height;

	siv::PerlinNoise::seed_type seed;

	float flowfield_strength = 0.01f;
	int particle_count = 10000;
	float particle_speed = 1.0f;
	float particle_size = 1.0f;
	unsigned char particle_strength = 1;

	float noise_height = 0;
	int noise_detail = 4;
	float x_mult = 0.02f;
	float y_mult = 0.02f;
	float z_mult = 0.02f;
	float z = 0.0f;


	siv::PerlinNoise perlin;
	std::vector<std::vector<Vector2>> flowfield;
	std::vector<Particle> particles;
	RenderTexture2D image;

public:
	Flowfield();
	void Update(int active_blend_mode);
	void ResetImage(Color background_color);
	void Reset(std::map<std::string, float> user_values);
	void InitParticles();
	void InitFlowfield();
};

