#pragma once
#include "raylib.h"
#include "Particle.h"
#include "../../PerlinNoise.hpp"
class Flowfield
{
public:
	const int kWindowWidth = 1600;
	const int kWindowHeight = 900;

	int scale = 20;

	std::vector<std::vector<Vector2>> flowfield;

	float flowfield_strength = 0.01f;
	int particle_count = 20000;
	float particle_speed = 1.0f;
	float particle_size = 1.0f;
	unsigned char particle_strength = 1;
	std::vector<Particle> particles;

	float noise_height = 0;
	int noise_detail = 6;
	float x_mult = 0.02f;
	float y_mult = 0.02f;
	float z_mult = 0.02f;
	float z = 0.0f;

	int render_width;
	int render_height;

	const siv::PerlinNoise perlin;

	RenderTexture2D image;

public:
	Flowfield();
	void Update();
};

