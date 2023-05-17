#include "Voronoi.h"
#include <random>
#include "../../Functions.h"
#include "../../json/JSONReader.h"

Voronoi::Voronoi()
{
	std::cout << "Voronoi\n";
	std::cout << "App directory: " << GetApplicationDirectory() << '\n';
	std::cout << "Attempting to load voronoi.json\n";
	default_settings = JSONReader::LoadSettingsFromJson(std::string("cfg/voronoi.json"));
	user_settings = default_settings;
	Voronoi::InitializeDefaultVariablesFromSettings();
	Voronoi::ApplySettings();
}

std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>>& Voronoi::GetUserSettings()
{
	return user_settings;
}

void Voronoi::InitializeDefaultVariablesFromSettings()
{
	window_width = std::get<float>(user_settings.second["Window Width"].value);
	window_height = std::get<float>(user_settings.second["Window Height"].value);
	seed = std::get<float>(user_settings.second["Seed"].value);
	seed_count = std::get<float>(user_settings.second["Seed Count"].value);
	render_points = (bool)std::get<float>(user_settings.second["Render Points"].value);
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
	InitializeDefaultVariablesFromSettings();
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
