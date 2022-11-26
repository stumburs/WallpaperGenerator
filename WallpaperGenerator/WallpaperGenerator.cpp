#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <iostream>
#include <vector>
#include "include/Functions.h"
#include "include/PerlinNoise.hpp"
#include "include/style/flowfield/Particle.h"
#include "include/style/flowfield/Flowfield.h"
#include "include/style/shapes/Shapes.h"

const int kWindowWidth = 1600;
const int kWindowHeight = 900;

bool shader_on = false;

Rectangle preview_rect = { kWindowWidth - 850, 50, 800, 450 };

Rectangle shader_checkbox = { kWindowWidth - 850, 525, 40, 40 };
Rectangle blend_slider = { kWindowWidth - 650, 525, 400, 40 };
Rectangle save_image = { kWindowWidth - 850, 585, 40, 40 };
Rectangle reset_image = { kWindowWidth - 650, 585, 40, 40 };

enum Algorithm
{
	NONE = 0,
	FLOWFIELD,
	SHAPES
};

int active_algorithm = Algorithm::NONE;
int active_blend_mode = BlendMode::BLEND_ALPHA;


int main()
{
	// Init window
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(kWindowWidth, kWindowHeight, "Wallpaper Generator");
    SetTargetFPS(0);

	// Init shader
	Shader shader = LoadShader(0, "./include/shader/PIXELIZER.fs");

	// Create flowfield preset thing
	Flowfield f;
	Shapes s;

	//GuiLoadStyle("include/style/gui/jungle.rgs");
	// https://www.color-hex.com/color-palette/28549
	GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
	//GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x1e2124);

	while (!WindowShouldClose())
	{
		// Update
		{
			switch (active_algorithm)
			{
			case NONE:
				break;
			case FLOWFIELD:
				// Update flowfield
				f.Update(active_blend_mode);
				break;
			case SHAPES:
				// Update shapes
				s.Update();
				break;
			default:
				break;
			}
			
		}

		// Draw
		BeginDrawing();
		{
			ClearBackground(RAYWHITE);

			switch (active_algorithm)
			{
			case NONE:
				if (GuiLabelButton({ 40, 40, 100, 40 }, "NONE"))
				{
					active_algorithm = Algorithm::NONE;
				}
				if (GuiLabelButton({ 40, 100, 100, 40 }, "FLOWFIELD"))
				{
					active_algorithm = Algorithm::FLOWFIELD;
				}
				if (GuiLabelButton({ 40, 160, 100, 40 }, "SHAPES"))
				{
					active_algorithm = Algorithm::SHAPES;
				}
				break;
			case FLOWFIELD:
				// Preview Box
				DrawRectangleRec(preview_rect, BLACK);

				shader_on = GuiCheckBox(shader_checkbox, "SHADER", shader_on);

				active_blend_mode = GuiScrollBar(blend_slider, active_blend_mode, 0, 6);

				if (GuiLabelButton(reset_image, "Reset flowfield"))
				{
					f.image = LoadRenderTexture(kWindowWidth, kWindowHeight);
					BeginTextureMode(f.image);
					ClearBackground(BLACK);
					EndTextureMode();
				}

				if (GuiLabelButton(save_image, "Save Image")) {
					Image img = LoadImageFromTexture(f.image.texture);
					ExportImage(img, "text.png");
				}

				switch (active_blend_mode)
				{
				case 0:
					DrawText("BLEND_ALPHA", blend_slider.x + blend_slider.width / 2 - (float)MeasureText("BLEND_ALPHA", 20) / 2, blend_slider.y + 10, 20, BLACK);
					break;
				case 1:
					DrawText("BLEND_ADDITIVE", blend_slider.x + blend_slider.width / 2 - (float)MeasureText("BLEND_ADDITIVE", 20) / 2, blend_slider.y + 10, 20, BLACK);
					break;
				case 2:
					DrawText("BLEND_MULTIPLIED", blend_slider.x + blend_slider.width / 2 - (float)MeasureText("BLEND_MULTIPLIED", 20) / 2, blend_slider.y + 10, 20, BLACK);
					break;
				case 3:
					DrawText("BLEND_ADD_COLORS", blend_slider.x + blend_slider.width / 2 - (float)MeasureText("BLEND_ADD_COLORS", 20) / 2, blend_slider.y + 10, 20, BLACK);
					break;
				case 4:
					DrawText("BLEND_SUBTRACT_COLORS", blend_slider.x + blend_slider.width / 2 - (float)MeasureText("BLEND_SUBTRACT_COLORS", 20) / 2, blend_slider.y + 10, 20, BLACK);
					break;
				case 5:
					DrawText("BLEND_ALPHA_PREMULTIPLY", blend_slider.x + blend_slider.width / 2 - (float)MeasureText("BLEND_ALPHA_PREMULTIPLY", 20) / 2, blend_slider.y + 10, 20, BLACK);
					break;
				case 6:
					DrawText("BLEND_CUSTOM", blend_slider.x + blend_slider.width / 2 - (float)MeasureText("BLEND_CUSTOM", 20) / 2, blend_slider.y + 10, 20, BLACK);
					break;
				default:
					break;
				}

				//BeginBlendMode(active_blend_mode);

				// Apply shader
				if (shader_on)
					BeginShaderMode(shader);

				DrawTexturePro(f.image.texture, { 0, 0, (float)f.image.texture.width, (float)-f.image.texture.height }, preview_rect, { 0, 0 }, 0.0f, WHITE);
				EndShaderMode();

				//EndBlendMode();
				DrawText("Preview", kWindowWidth - 425 - MeasureText("PREVIEW", 40) / 2, 250, 40, { 255, 255, 255, 60 });
				
				break;
			case SHAPES:
				// Preview Box
				DrawRectangle(kWindowWidth - 850, 50, 800, 450, BLACK);

				shader_on = GuiCheckBox({ kWindowWidth - 850, 525, 40, 40 }, "SHADER", shader_on);

				active_blend_mode = GuiScrollBar({ 60, 60, 400, 40 }, active_blend_mode, 0, 6);

				switch (active_blend_mode)
				{
				case 0:
					DrawText("BLEND_ALPHA", 180, 100, 20, BLACK);
					break;
				case 1:
					DrawText("BLEND_ADDITIVE", 180, 100, 20, BLACK);
					break;
				case 2:
					DrawText("BLEND_MULTIPLIED", 180, 100, 20, BLACK);
					break;
				case 3:
					DrawText("BLEND_ADD_COLORS", 180, 100, 20, BLACK);
					break;
				case 4:
					DrawText("BLEND_SUBTRACT_COLORS", 180, 100, 20, BLACK);
					break;
				case 5:
					DrawText("BLEND_ALPHA_PREMULTIPLY", 180, 100, 20, BLACK);
					break;
				case 6:
					DrawText("BLEND_CUSTOM", 180, 100, 20, BLACK);
					break;
				default:
					break;
				}

				BeginBlendMode(active_blend_mode);

				// Apply shader
				if (shader_on)
					BeginShaderMode(shader);

				DrawTexturePro(s.image.texture, { 0, 0, (float)s.image.texture.width, (float)-s.image.texture.height }, { kWindowWidth - 850, 50, 800, 450 }, { 0, 0 }, 0.0f, WHITE);
				EndShaderMode();

				EndBlendMode();
				DrawText("Preview", kWindowWidth - 425 - MeasureText("PREVIEW", 40) / 2, 250, 40, { 255, 255, 255, 60 });
				break;
			default:
				break;
			}

			DrawFPS(20, 20);

			if (shader_on)
				DrawText("YES", 20, 40, 20, GREEN);

		}
		EndDrawing();
	}
    CloseWindow();
}