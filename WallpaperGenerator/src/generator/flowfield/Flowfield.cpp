#include "Flowfield.h"
#include "../../PerlinNoise.hpp"
#include "../../Functions.h"
#include <string>
#include <algorithm>
#include <cmath>
#include <iostream>
Flowfield::Flowfield()
{
	Flowfield::Init();
}

std::vector<Generator::Setting>& Flowfield::GetUserSettings()
{
	return user_settings;
}

void Flowfield::Init()
{
	window_width = std::stoi(user_settings[0].string_value);
	window_height = std::stoi(user_settings[1].string_value);
	seed = std::stoi(user_settings[2].string_value);
	scale = user_settings[3].value;
	flowfield_strength = user_settings[4].value;
	particle_count = user_settings[5].value;
	particle_speed = user_settings[6].value;
	particle_strength = user_settings[7].value;
	noise_detail = user_settings[8].value;
	x_mult = user_settings[9].value;
	y_mult = user_settings[10].value;
	z_mult = user_settings[11].value;
	background_color = { (unsigned char)std::stoi(user_settings[12].string_value.substr(0, 3)), (unsigned char)std::stoi(user_settings[12].string_value.substr(3, 3)), (unsigned char)std::stoi(user_settings[12].string_value.substr(6, 3)) };
	background_a = user_settings[13].value;
	top = { (unsigned char)std::stoi(user_settings[14].string_value.substr(0, 3)), (unsigned char)std::stoi(user_settings[14].string_value.substr(3, 3)), (unsigned char)std::stoi(user_settings[14].string_value.substr(6, 3)), 255 };
	bottom = { (unsigned char)std::stoi(user_settings[15].string_value.substr(0, 3)), (unsigned char)std::stoi(user_settings[15].string_value.substr(3, 3)), (unsigned char)std::stoi(user_settings[15].string_value.substr(6, 3)), 255 };
	left = { (unsigned char)std::stoi(user_settings[16].string_value.substr(0, 3)), (unsigned char)std::stoi(user_settings[16].string_value.substr(3, 3)), (unsigned char)std::stoi(user_settings[16].string_value.substr(6, 3)), 255 };
	right = { (unsigned char)std::stoi(user_settings[17].string_value.substr(0, 3)), (unsigned char)std::stoi(user_settings[17].string_value.substr(3, 3)), (unsigned char)std::stoi(user_settings[17].string_value.substr(6, 3)), 255 };
	blend_mode = user_settings[18].value;

	perlin.reseed(seed);
	render_width = window_width / scale + 1;
	render_height = window_height / scale + 1;
	z = 0;

	color_vector.clear();
	// Init color vector to white
	for (int x = 0; x < window_height; x++)
	{
		std::vector<Color> y_vec;
		for (int y = 0; y < window_width; y++)
		{
			Color top_to_bottom = { 0 };
			top_to_bottom.r = SimpleLerp((float)top.r, (float)bottom.r, Map(x, 0, window_height - 1, 0, 1));
			top_to_bottom.g = SimpleLerp((float)top.g, (float)bottom.g, Map(x, 0, window_height - 1, 0, 1));
			top_to_bottom.b = SimpleLerp((float)top.b, (float)bottom.b, Map(x, 0, window_height - 1, 0, 1));

			Color left_to_right = { 0 };
			left_to_right.r = SimpleLerp((float)left.r, (float)right.r, Map(y, 0, window_width - 1, 0, 1));
			left_to_right.g = SimpleLerp((float)left.g, (float)right.g, Map(y, 0, window_width - 1, 0, 1));
			left_to_right.b = SimpleLerp((float)left.b, (float)right.b, Map(y, 0, window_width - 1, 0, 1));

			Color result = { 0 };
			result.r = SimpleClamp(((int)left_to_right.r + (int)top_to_bottom.r) / 2, 0, 255);
			result.g = SimpleClamp(((int)left_to_right.g + (int)top_to_bottom.g) / 2, 0, 255);
			result.b = SimpleClamp(((int)left_to_right.b + (int)top_to_bottom.b) / 2, 0, 255);
			result.a = particle_strength;
			y_vec.push_back({ result });
		}
		color_vector.push_back(y_vec);
	}

	// Particles
	particles.clear();
	for (int i = 0; i < particle_count; i++)
	{
		Particle p;
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

	image = LoadRenderTexture(window_width, window_height);
	return_image = LoadRenderTexture(window_width, window_height);
	BeginTextureMode(image);
	ClearBackground(BLANK);
	EndTextureMode();
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
			/*particles[i].DrawPixel({
				(unsigned char)Map(particles[i].pos.y, 0, window_height, 255, 128),
				0,
				(unsigned char)Map(particles[i].pos.y, 0, window_height , 128, 255),
				particle_strength });*/

		}
	}
	EndTextureMode();

	// Move through noise
	z += 0.06f;
}

void Flowfield::ApplySettings()
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
	scale = user_settings[3].value;
	flowfield_strength = user_settings[4].value;
	particle_count = user_settings[5].value;
	particle_speed = user_settings[6].value;
	particle_strength = user_settings[7].value;
	noise_detail = user_settings[8].value;
	x_mult = user_settings[9].value;
	y_mult = user_settings[10].value;
	z_mult = user_settings[11].value;
	background_color = { (unsigned char)std::stoi(user_settings[12].string_value.substr(0, 3)), (unsigned char)std::stoi(user_settings[12].string_value.substr(3, 3)), (unsigned char)std::stoi(user_settings[12].string_value.substr(6, 3)) };
	background_a = user_settings[13].value;
	top = { (unsigned char)std::stoi(user_settings[14].string_value.substr(0, 3)), (unsigned char)std::stoi(user_settings[14].string_value.substr(3, 3)), (unsigned char)std::stoi(user_settings[14].string_value.substr(6, 3)), 255 };
	bottom = { (unsigned char)std::stoi(user_settings[15].string_value.substr(0, 3)), (unsigned char)std::stoi(user_settings[15].string_value.substr(3, 3)), (unsigned char)std::stoi(user_settings[15].string_value.substr(6, 3)), 255 };
	left = { (unsigned char)std::stoi(user_settings[16].string_value.substr(0, 3)), (unsigned char)std::stoi(user_settings[16].string_value.substr(3, 3)), (unsigned char)std::stoi(user_settings[16].string_value.substr(6, 3)), 255 };
	right = { (unsigned char)std::stoi(user_settings[17].string_value.substr(0, 3)), (unsigned char)std::stoi(user_settings[17].string_value.substr(3, 3)), (unsigned char)std::stoi(user_settings[17].string_value.substr(6, 3)), 255 };
	blend_mode = user_settings[18].value;

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
			top_to_bottom.r = SimpleLerp((float)top.r, (float)bottom.r, Map(x, 0, window_height - 1, 0, 1));
			top_to_bottom.g = SimpleLerp((float)top.g, (float)bottom.g, Map(x, 0, window_height - 1, 0, 1));
			top_to_bottom.b = SimpleLerp((float)top.b, (float)bottom.b, Map(x, 0, window_height - 1, 0, 1));
			top_to_bottom.a = 255;

			Color left_to_right = { 0 };
			left_to_right.r = SimpleLerp((float)left.r, (float)right.r, Map(y, 0, window_width - 1, 0, 1));
			left_to_right.g = SimpleLerp((float)left.g, (float)right.g, Map(y, 0, window_width - 1, 0, 1));
			left_to_right.b = SimpleLerp((float)left.b, (float)right.b, Map(y, 0, window_width - 1, 0, 1));
			left_to_right.a = 255;

			Color result = { 0 };
			result.r = SimpleClamp(((int)left_to_right.r + (int)top_to_bottom.r) / 2, 0, 255);
			result.g = SimpleClamp(((int)left_to_right.g + (int)top_to_bottom.g) / 2, 0, 255);
			result.b = SimpleClamp(((int)left_to_right.b + (int)top_to_bottom.b) / 2, 0, 255);
			result.a = particle_strength;
			y_vec.push_back({ result });
		}
		color_vector.push_back(y_vec);
	}

	//std::cout << "TOP: " << static_cast<unsigned>(top.r) << " " << static_cast<unsigned>(top.g) << " " << static_cast<unsigned>(top.b) << " " << static_cast<unsigned>(top.a) << "\n";
	//std::cout << "BOTTOM: " << static_cast<unsigned>(bottom.r) << " " << static_cast<unsigned>(bottom.g) << " " << static_cast<unsigned>(bottom.b) << " " << static_cast<unsigned>(bottom.a) << "\n";
	//std::cout << "LEFT: " << static_cast<unsigned>(left.r) << " " << static_cast<unsigned>(left.g) << " " << static_cast<unsigned>(left.b) << " " << static_cast<unsigned>(left.a) << "\n";
	//std::cout << "RIGHT: " << static_cast<unsigned>(right.r) << " " << static_cast<unsigned>(right.g) << " " << static_cast<unsigned>(right.b) << " " << static_cast<unsigned>(right.a) << "\n";


	// Particles
	particles.clear();
	for (int i = 0; i < particle_count; i++)
	{
		Particle p;
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
	BeginBlendMode(blend_mode);
	DrawTexturePro(image.texture, { 0, 0, (float)image.texture.width, (float)image.texture.height }, { 0, 0, (float)return_image.texture.width, (float)return_image.texture.height }, { 0, 0 }, 0.0f, WHITE);
	EndBlendMode();
	EndTextureMode();
	return return_image.texture;
}