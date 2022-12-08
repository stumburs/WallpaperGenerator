#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION
#include <iostream>
#include "gui/Gui.h"
#include "generator/Generator.h"

const int kWindowWidth = 1600;
const int kWindowHeight = 900;

int main()
{
	// Init window
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(kWindowWidth, kWindowHeight, "Wallpaper Generator");
	SetTargetFPS(60);

	// Init generator
	Generator generator(Generator::Generators::NONE);

	// Init GUI
	Gui gui(kWindowWidth, kWindowHeight, &generator);

	while (!WindowShouldClose())
	{
		// Update
		{
			generator.Update();
			gui.Update();
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