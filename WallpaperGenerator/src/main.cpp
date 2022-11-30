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
	Generator generator(Generator::Generators::NONE);

	// Init GUI
	Gui gui(kWindowWidth, kWindowHeight, &generator);

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