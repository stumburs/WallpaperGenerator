#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "libraries/raygui-3.2/raygui.h"
#undef RAYGUI_IMPLEMENTATION
#include <iostream>
#include "gui/Gui.h"

const int kWindowWidth = 1600;
const int kWindowHeight = 900;

int main()
{
	// Init window
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(kWindowWidth, kWindowHeight, "Wallpaper Generator");
	SetTargetFPS(60);

	// Init GUI
	Gui gui(kWindowWidth, kWindowHeight);

	while (!WindowShouldClose())
	{
		// Update
		gui.Update();

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