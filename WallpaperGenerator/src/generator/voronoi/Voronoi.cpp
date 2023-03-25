#include "Voronoi.h"
#include <random>
#include "../../Functions.h"

Voronoi::Voronoi()
{
	Voronoi::Init();
	Voronoi::GenerateRandomSeeds();
}

std::vector<Generator::Setting>& Voronoi::GetUserSettings()
{
	return user_settings;
}

void Voronoi::GenerateRandomSeeds()
{
	seeds.clear();
	srand(seed);
	for (int i = 0; i < seed_count; i++)
	{
		Vector2 pos = { rand() % window_width, rand() % window_width };
		seeds.push_back(pos);
	}
}

int Voronoi::SqrDist(int x1, int y1, int x2, int y2)
{
	int dx = x1 - x2;
	int dy = y1 - y2;
	return dx * dx + dy * dy;
}

void Voronoi::Update()
{
	if (!rendered)
	{
		GenerateRandomSeeds();
		BeginTextureMode(image);
		ClearBackground(BLACK);
		for (int y = 0; y < window_width; y++)
		{
			for (int x = 0; x < window_width; x++)
			{
				int j = 0;
				for (int i = 0; i < seed_count; i++)
				{
					if (SqrDist(seeds[i].x, seeds[i].y, x, y) < SqrDist(seeds[j].x, seeds[j].y, x, y))
					{
						j = i;
					}
				}
				int palette_index = j % palette_count;
				DrawPixel(x, y, palette[palette_index]);
			}
		}

		if (render_points)
		{
			for (Vector2 const& seed : seeds)
				DrawCircle(seed.x, seed.y, 4, BLACK);
		}
		EndTextureMode();
		rendered = true;
	}
}

void Voronoi::Init()
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
		seed_count = std::stoi(user_settings[3].string_value);
	}
	catch (const std::exception&) {}
	render_points = user_settings[4].value;
	rendered = false;

	// Init texture
	UnloadRenderTexture(image);
	image = LoadRenderTexture(window_width, window_height);
	BeginTextureMode(image);
	ClearBackground(BLACK);
	EndTextureMode();
}

void Voronoi::ApplySettings()
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
		seed_count = std::stoi(user_settings[3].string_value);
	}
	catch (const std::exception&) {}
	render_points = user_settings[4].value;
	rendered = false;

	// Init texture
	UnloadRenderTexture(image);
	image = LoadRenderTexture(window_width, window_height);
	BeginTextureMode(image);
	ClearBackground(background_color);
	EndTextureMode();
}

void Voronoi::ResetSettings()
{
	user_settings = default_settings;
}

Texture2D Voronoi::GetImage()
{
	return image.texture;
}
