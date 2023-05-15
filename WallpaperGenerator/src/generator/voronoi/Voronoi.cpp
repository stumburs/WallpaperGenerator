#include "Voronoi.h"
#include <random>
#include "../../Functions.h"
#include "../../json/JSONReader.h"

Voronoi::Voronoi()
{
	//default_settings = JSONReader::LoadSettingsFromJson()
	default_settings =
	{
		{ "Window Width", 1920, 0, std::pair{ static_cast<float>(2), static_cast<float>(1920) }, "Final image horizontal resolution", InputType::GUI_TEXT_BOX, "1920", false },
		{ "Window Height", 1080, 0, std::pair{ static_cast<float>(2), static_cast<float>(1080) }, "Final image vertical resolution", InputType::GUI_TEXT_BOX, "1080", false },
		{ "Seed", 69420, 0, std::pair{static_cast<float>(0), static_cast<float>(UINT32_MAX)}, "Random noise seed", InputType::GUI_TEXT_BOX, "69420", false},
		{ "Seed Count", 20, 0, std::pair{ static_cast<float>(1), static_cast<float>(100) }, "Amount of seeds for diagram", InputType::GUI_TEXT_BOX, "20", false },
		{ "Render Points", 1, 0, std::pair{ static_cast<float>(0), static_cast<float>(1) }, "Whether to render points", InputType::GUI_CHECK_BOX, "1", false }
	};
	user_settings = default_settings;
	Voronoi::ApplySettings();
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
		Vector2 pos = { static_cast<float>(rand() % window_width), static_cast<float>(rand() % window_height) };
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
					if (SqrDist(static_cast<int>(seeds[i].x), static_cast<int>(seeds[i].y), x, y) < SqrDist(static_cast<int>(seeds[j].x), static_cast<int>(seeds[j].y), x, y))
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
			for (Vector2 seed : seeds)
				DrawCircle(static_cast<int>(seed.x), static_cast<int>(seed.y), 4, BLACK);
		}
		EndTextureMode();
		rendered = true;
	}
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
