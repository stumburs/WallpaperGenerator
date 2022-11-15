#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <iostream>
#include <vector>
#include "include/style/flowfield/Particle.h"
#include "include/Functions.h"
#include "include/PerlinNoise.hpp"
#include "include/style/flowfield/Flowfield.h"

const int kWindowWidth = 1600;
const int kWindowHeight = 900;

bool shader_on = false;

int main()
{
	// Init window
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(kWindowWidth, kWindowHeight, "Wallpaper Generator");
    SetTargetFPS(0);

	// Init shader
	Shader shader = LoadShader(0, "./include/shader/PIXELIZER.fs");
	//float resolution[2] = { kWindowWidth, kWindowHeight };

	// Create flowfield preset thing
	Flowfield f;

	GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
	GuiLoadStyle("/include/style/gui/dark.rgs");

	while (!WindowShouldClose())
	{
		// Update
		{
			// Enable/disable shader
			/*if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				shader_on = !shader_on;*/

			// Update flowfield
			f.Update();
		}

		// Draw
		BeginDrawing();
		{
			ClearBackground(RAYWHITE);


			// Preview Box
			DrawRectangle(kWindowWidth - 850, 50, 800, 450, BLACK);
			DrawText("Preview", kWindowWidth - 425 - MeasureText("PREVIEW", 40) / 2, 250, 40, {255, 255, 255, 60});
			shader_on = GuiCheckBox({ kWindowWidth - 850, 525, 40, 40 }, "SHADER", shader_on);

			BeginBlendMode(BLEND_ADD_COLORS);

			// Apply shader
			if (shader_on)
				BeginShaderMode(shader);

			//DrawTextureRec(f.image.texture, { 0, 0, (float)f.image.texture.width, (float)-f.image.texture.height }, { 0, 0 }, WHITE);
			DrawTexturePro(f.image.texture, { 0, 0, (float)f.image.texture.width, (float)-f.image.texture.height }, { kWindowWidth - 850, 50, 800, 450}, {0, 0}, 0.0f, WHITE);
			EndShaderMode();

			EndBlendMode();
			DrawFPS(20, 20);

			if (shader_on)
				DrawText("YES", 20, 40, 20, GREEN);

		}
		EndDrawing();
	}
    CloseWindow();
}