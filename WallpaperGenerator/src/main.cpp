#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION
#include <iostream>
#include <vector>
#include "Functions.hpp"
#include "PerlinNoise.hpp"
#include "generator/flowfield/Flowfield.h"
#include "generator/shapes/Shapes.h"
#include "gui/Gui.h"

#include "generator/Generator.h"

const int kWindowWidth = 1600;
const int kWindowHeight = 900;

bool shader_on = false;

std::map<std::string, float> user_values = {
	{ "window_width", 1600 },
	{ "window_height", 900 },
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
	{ "z_mult", 0.02f}
 };

std::map<std::string, float> default_values = user_values;

int active_blend_mode = BlendMode::BLEND_ALPHA;

Vector2 scroll_pos = { 0 };

int main()
{
	// Init window
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(kWindowWidth, kWindowHeight, "Wallpaper Generator");
	SetTargetFPS(0);

	// Init shader
	//Shader shader = LoadShader(0, "./include/shader/PIXELIZER.fs");

	// Init generator
	Generator generator(Generator::Generators::FLOWFIELD);

	// Init GUI
	Gui gui(kWindowWidth, kWindowHeight, user_values);

	while (!WindowShouldClose())
	{
		// Update
		{
			generator.Update();
			gui.Update(generator.GetImage());
		}

		// Draw
		BeginDrawing();
		{
			ClearBackground(RAYWHITE);
			gui.Draw();
			DrawFPS(20, 20);
		}
		EndDrawing();
	}
	CloseWindow();
}