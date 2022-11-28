#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION
#include <iostream>
#include <vector>
#include "Functions.hpp"
#include "PerlinNoise.hpp"
#include "style/flowfield/Flowfield.h"
#include "style/shapes/Shapes.h"
#include "gui/Gui.h"

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
	Shader shader = LoadShader(0, "./include/shader/PIXELIZER.fs");


	// Init algorithms
	Flowfield f;
	Shapes s;
	// Init GUI
	Gui gui(kWindowWidth, kWindowHeight, user_values, f.image.texture);

	//GuiLoadStyle("include/style/gui/jungle.rgs");
	// https://www.color-hex.com/color-palette/28549
	//GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x1e2124);

	while (!WindowShouldClose())
	{
		// Update
		{
			switch (gui.active_algorithm)
			{

			case Gui::Algorithm::NONE:
				break;

			case Gui::Algorithm::FLOWFIELD:
				// Update flowfield
				f.Update(active_blend_mode);
				break;

			case Gui::Algorithm::SHAPES:
				// Update shapes
				s.Update(active_blend_mode);
				break;

			default:
				break;
			}
			
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