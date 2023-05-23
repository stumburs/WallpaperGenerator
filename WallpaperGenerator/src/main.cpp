#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "libraries/raygui-3.2/raygui.h"
#undef RAYGUI_IMPLEMENTATION
#include <iostream>
#include "gui/Gui.h"

int window_width = 1600;
int window_height = 900;

int main()
{
	// Init window
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(window_width, window_height, "Wallpaper Generator");
	SetTargetFPS(60);

	// Init GUI
	Gui gui(window_width, window_height);

	while (!WindowShouldClose())
	{
		// Update
		gui.Update();

		// Draw
		BeginDrawing();
		{
			ClearBackground({ 126, 127, 130, 255 });
			gui.Draw();
			DrawFPS(20, 20);
		}
		EndDrawing();
	}
	CloseWindow();
}