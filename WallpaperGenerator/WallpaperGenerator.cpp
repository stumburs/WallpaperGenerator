#include <raylib.h>
#include <iostream>
#include <vector>
#include "include/style/flowfield/Particle.h"
#include "include/Functions.h"
#include "include/PerlinNoise.hpp"
#include "include/style/flowfield/Flowfield.h"

const int kWindowWidth = 1600;
const int kWindowHeight = 900;

bool to_shader = false;

int main()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(kWindowWidth, kWindowHeight, "Wallpaper Generator");
    SetTargetFPS(0);

	Shader shader = LoadShader(0, "./include/shader/PIXELIZER.fs");
	float resolution[2] = { kWindowWidth, kWindowHeight };

	Flowfield f;

    bool background_cleared = false;

	while (!WindowShouldClose())
	{
		// Update
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				to_shader = !to_shader;
			}

			f.Update();

		}

		// Draw
		BeginDrawing();
		{
			ClearBackground(BLACK);
			BeginBlendMode(BLEND_ADD_COLORS);

			if (to_shader)
				BeginShaderMode(shader);

			DrawTextureRec(f.image.texture, { 0, 0, (float)f.image.texture.width, (float)-f.image.texture.height }, { 0, 0 }, WHITE);
			EndShaderMode();

			EndBlendMode();
			DrawFPS(20, 20);

			if (to_shader)
				DrawText("YES", 20, 40, 20, GREEN);

		}
		EndDrawing();
	}
    CloseWindow();
}