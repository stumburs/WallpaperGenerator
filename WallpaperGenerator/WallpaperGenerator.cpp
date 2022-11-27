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

// Main menu
Rectangle create_rect = { kWindowWidth / 2 - 450, kWindowHeight / 2 - 50, 400, 100 };
Rectangle view_rect = { kWindowWidth / 2 + 50, kWindowHeight / 2 - 50, 400, 100 };

// Create menu
Rectangle flowfield_rect = { kWindowWidth / 2 - 450, kWindowHeight / 2 - 50, 400, 100 };
Rectangle shapes_rect = { kWindowWidth / 2 + 50, kWindowHeight / 2 - 50, 400, 100 };

// Generate
Rectangle preview_rect = { kWindowWidth - 850, 50, 800, 450 };
Rectangle shader_checkbox = { kWindowWidth - 850, 525, 40, 40 };
Rectangle blend_slider = { kWindowWidth - 650, 525, 400, 40 };
Rectangle save_image = { kWindowWidth - 850, 585, 40, 40 };
Rectangle reset_image = { kWindowWidth - 650, 585, 40, 40 };
Rectangle update_settings = { kWindowWidth - 850, 645, 40, 40 };
Rectangle reset_settings = { kWindowWidth - 650, 645, 40, 40 };

// Flowfield user settings
Rectangle window_width_rect = { 60, 40, 400, 40 };
Rectangle window_height_rect = { 60, 100, 400, 40 };
Rectangle scale_rect = { 60, 160, 400, 40 };
Rectangle seed_rect = { 60, 220, 400, 40 };
Rectangle flowfield_strength_rect = { 60, 280, 400, 40 };
Rectangle particle_count_rect = { 60, 340, 400, 40 };
Rectangle particle_speed_rect = { 60, 400, 400, 40 };
Rectangle particle_size_rect = { 60, 460, 400, 40 };
Rectangle particle_strength_rect = { 60, 520, 400, 40 };
Rectangle noise_height_rect = { 60, 580, 400, 40 };
Rectangle noise_detail_rect = { 60, 640, 400, 40 };
Rectangle x_mult_rect = { 60, 700, 400, 40 };
Rectangle y_mult_rect = { 60, 760, 400, 40 };
Rectangle z_mult_rect = { 60, 820, 400, 40 };
Rectangle z_rect = { 60, 880, 400, 40 };

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

enum Menu
{
	MAIN = 0,
	CREATE,
	GENERATOR
};

enum Algorithm
{
	NONE = 0,
	FLOWFIELD,
	SHAPES
};

int active_menu = Menu::MAIN;
int active_algorithm = Algorithm::NONE;
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

			switch (active_menu)
			{
			case Menu::MAIN:
				if (GuiButton(create_rect, "Create"))
				{
					active_menu = Menu::CREATE;
				}
				GuiButton(view_rect, "View");
				break;
			case Menu::CREATE:
				if (GuiButton(flowfield_rect, "Flowfield"))
				{
					active_menu = Menu::GENERATOR;
					active_algorithm = Algorithm::FLOWFIELD;
				}
				if (GuiButton(shapes_rect, "Shapes"))
				{
					active_menu = Menu::GENERATOR;
					active_algorithm = Algorithm::SHAPES;
				}
				break;
			case Menu::GENERATOR:
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
					DrawRectangleRec(preview_rect, WHITE);

					shader_on = GuiCheckBox(shader_checkbox, "SHADER", shader_on);

					active_blend_mode = GuiSliderBar(blend_slider, NULL, NULL, active_blend_mode, 0, 6);


					/*
					*  !!!!!!!!!!!!!!!
					* 
					*	REFACTOR ASAP
					* 
					*  !!!!!!!!!!!!!!!
					*/

					Rectangle scissor_area = GuiScrollPanel({ 50, 50, 650, 800 }, NULL, { 50, 50, 635, 1000 }, &scroll_pos);
					BeginScissorMode(scissor_area.x, scissor_area.y, scissor_area.width, scissor_area.height);

					// User values
					user_values["window_width"] = (int)GuiSliderBar({ window_width_rect.x + 50, window_width_rect.y + 50 + scroll_pos.y, window_width_rect.width, window_width_rect.height }, TextFormat("%d", (int)user_values["window_width"]), "Image Width", (int)user_values["window_width"], 1, 1920);
					user_values["window_height"] = (int)GuiSliderBar({ window_height_rect.x + 50, window_height_rect.y + 50 + scroll_pos.y, window_height_rect.width, window_height_rect.height }, TextFormat("%d", (int)user_values["window_height"]), "Image Height", (int)user_values["window_height"], 1, 1080);
					user_values["scale"] = (int)GuiSliderBar({ scale_rect.x + 50, scale_rect.y + 50 + scroll_pos.y, scale_rect.width, scale_rect.height }, TextFormat("%d", (int)user_values["scale"]), "Image Scale", (int)user_values["scale"], 1, 40);
					//GuiDrawText("(seed input)", seed_rect, GuiTextAlignment::TEXT_ALIGN_CENTER, BLACK);
					user_values["flowfield_strength"] = GuiSliderBar({ flowfield_strength_rect.x + 50, flowfield_strength_rect.y + 50 + scroll_pos.y, flowfield_strength_rect.width, flowfield_strength_rect.height }, TextFormat("%0.3f", user_values["flowfield_strength"]), "Flowfield Strength", user_values["flowfield_strength"], 0.001f, 0.1f);
					user_values["particle_count"] = (int)GuiSliderBar({ particle_count_rect.x + 50, particle_count_rect.y + 50 + scroll_pos.y, particle_count_rect.width, particle_count_rect.height }, TextFormat("%d", (int)user_values["particle_count"]), "Particle count", (int)user_values["particle_count"], 500, 50000);
					user_values["particle_speed"] = GuiSliderBar({ particle_speed_rect.x + 50, particle_speed_rect.y + 50 + scroll_pos.y, particle_speed_rect.width, particle_speed_rect.height }, TextFormat("%0.1f", user_values["particle_speed"]), "Particle Speed", user_values["particle_speed"], 0.0f, 2.0f);
					//GuiDrawText("(particle size input)", particle_size_rect, GuiTextAlignment::TEXT_ALIGN_CENTER, BLACK);
					user_values["particle_strength"] = (unsigned char)GuiSliderBar({ particle_strength_rect.x + 50, particle_strength_rect.y + 50 + scroll_pos.y, particle_strength_rect.width, particle_strength_rect.height }, TextFormat("%d", (unsigned char)user_values["particle_strength"]), "Particle Strength", (unsigned char)user_values["particle_strength"], 1, 16);
					//GuiDrawText("(noise height input)", noise_height_rect, GuiTextAlignment::TEXT_ALIGN_CENTER, BLACK);
					user_values["noise_detail"] = (int)GuiSliderBar({ noise_detail_rect.x + 50, noise_detail_rect.y + 50 + scroll_pos.y, noise_detail_rect.width, noise_detail_rect.height }, TextFormat("%d", (int)user_values["noise_detail"]), "Noise Detail", (int)user_values["noise_detail"], 1, 16);
					user_values["x_mult"] = GuiSliderBar({ x_mult_rect.x + 50, x_mult_rect.y + 50 + scroll_pos.y, x_mult_rect.width, x_mult_rect.height }, TextFormat("%0.2f", user_values["x_mult"]), "X-Axis Multiplier", user_values["x_mult"], 0.01f, 0.2f);
					user_values["y_mult"] = GuiSliderBar({ y_mult_rect.x + 50, y_mult_rect.y + 50 + scroll_pos.y, y_mult_rect.width, y_mult_rect.height }, TextFormat("%0.2f", user_values["y_mult"]), "Y-Axis Multiplier", user_values["y_mult"], 0.01f, 0.2f);
					user_values["z_mult"] = GuiSliderBar({ z_mult_rect.x + 50, z_mult_rect.y + 50 + scroll_pos.y, z_mult_rect.width, z_mult_rect.height }, TextFormat("%0.2f", user_values["z_mult"]), "Z-Axis Multiplier", user_values["z_mult"], 0.01f, 0.2f);

					if (CheckCollisionPointRec(GetMousePosition(), { window_width_rect.x + 50, window_width_rect.y + 50 + scroll_pos.y, window_width_rect.width, window_width_rect.height }))
						GuiDrawText("Final image horizontal resolution", { GetMousePosition().x, GetMousePosition().y - 10, 0, 0 }, TEXT_ALIGN_CENTER, DARKGRAY);
					if (CheckCollisionPointRec(GetMousePosition(), { window_height_rect.x + 50, window_height_rect.y + 50 + scroll_pos.y, window_height_rect.width, window_height_rect.height }))
						GuiDrawText("Final image horizontal resolution", { GetMousePosition().x, GetMousePosition().y - 10, 0, 0 }, TEXT_ALIGN_CENTER, DARKGRAY);
					if (CheckCollisionPointRec(GetMousePosition(), { scale_rect.x + 50, scale_rect.y + 50 + scroll_pos.y, scale_rect.width, scale_rect.height }))
						GuiDrawText("Lower number = more detail, but worse performance", { GetMousePosition().x, GetMousePosition().y - 10, 0, 0 }, TEXT_ALIGN_CENTER, DARKGRAY);
					if (CheckCollisionPointRec(GetMousePosition(), { flowfield_strength_rect.x + 50, flowfield_strength_rect.y + 50 + scroll_pos.y, flowfield_strength_rect.width, flowfield_strength_rect.height }))
						GuiDrawText("How accurately particles follow the flowfield", { GetMousePosition().x, GetMousePosition().y - 10, 0, 0 }, TEXT_ALIGN_CENTER, DARKGRAY);
					if (CheckCollisionPointRec(GetMousePosition(), { particle_count_rect.x + 50, particle_count_rect.y + 50 + scroll_pos.y, particle_count_rect.width, particle_count_rect.height }))
						GuiDrawText("Amount of particles", { GetMousePosition().x, GetMousePosition().y - 10, 0, 0 }, TEXT_ALIGN_CENTER, DARKGRAY);
					if (CheckCollisionPointRec(GetMousePosition(), { particle_speed_rect.x + 50, particle_speed_rect.y + 50 + scroll_pos.y, particle_speed_rect.width, particle_speed_rect.height }))
						GuiDrawText("How quickly particles move", { GetMousePosition().x, GetMousePosition().y - 10, 0, 0 }, TEXT_ALIGN_CENTER, DARKGRAY);
					if (CheckCollisionPointRec(GetMousePosition(), { particle_strength_rect.x + 50, particle_strength_rect.y + 50 + scroll_pos.y, particle_strength_rect.width, particle_strength_rect.height }))
						GuiDrawText("Particle color intensity", { GetMousePosition().x, GetMousePosition().y - 10, 0, 0 }, TEXT_ALIGN_CENTER, DARKGRAY);
					if (CheckCollisionPointRec(GetMousePosition(), { noise_detail_rect.x + 50, noise_detail_rect.y + 50 + scroll_pos.y, noise_detail_rect.width, noise_detail_rect.height }))
						GuiDrawText("Perlin noise octaves. Higher number = worse performance", { GetMousePosition().x, GetMousePosition().y - 10, 0, 0 }, TEXT_ALIGN_CENTER, DARKGRAY);
					if (CheckCollisionPointRec(GetMousePosition(), { x_mult_rect.x + 50, x_mult_rect.y + 50 + scroll_pos.y, x_mult_rect.width, x_mult_rect.height }))
						GuiDrawText("Noise x-axis multiplier", { GetMousePosition().x, GetMousePosition().y - 10, 0, 0 }, TEXT_ALIGN_CENTER, DARKGRAY);
					if (CheckCollisionPointRec(GetMousePosition(), { y_mult_rect.x + 50, y_mult_rect.y + 50 + scroll_pos.y, y_mult_rect.width, y_mult_rect.height }))
						GuiDrawText("Noise y-axis multiplier", { GetMousePosition().x, GetMousePosition().y - 10, 0, 0 }, TEXT_ALIGN_CENTER, DARKGRAY);
					if (CheckCollisionPointRec(GetMousePosition(), { z_mult_rect.x + 50, z_mult_rect.y + 50 + scroll_pos.y, z_mult_rect.width, z_mult_rect.height }))
						GuiDrawText("Noise z-axis multiplier", { GetMousePosition().x, GetMousePosition().y - 10, 0, 0 }, TEXT_ALIGN_CENTER, DARKGRAY);

					EndScissorMode();

					if (GuiLabelButton(reset_image, "Reset flowfield"))
					{
						f.ResetImage(BLACK);
					}

					if (GuiLabelButton(save_image, "Save Image")) {
						Image img = LoadImageFromTexture(f.image.texture);
						ExportImage(img, "text.png");
					}

					if (GuiLabelButton(update_settings, "Update settings")) {
						f.Reset(user_values);
					}

					if (GuiLabelButton(reset_settings, "Reset settings")) {
						user_values = default_values;
						f.Reset(user_values);
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

					// Apply shader
					if (shader_on)
						BeginShaderMode(shader);

					DrawTexturePro(f.image.texture, { 0, 0, (float)f.image.texture.width, (float)-f.image.texture.height }, preview_rect, { 0, 0 }, 0.0f, WHITE);
					EndShaderMode();

					DrawText("Preview", kWindowWidth - 425 - MeasureText("PREVIEW", 40) / 2, 250, 40, { 255, 255, 255, 60 });

					break;
				case SHAPES:
					// Preview Box
					DrawRectangle(kWindowWidth - 850, 50, 800, 450, BLACK);

					shader_on = GuiCheckBox({ kWindowWidth - 850, 525, 40, 40 }, "SHADER", shader_on);

					active_blend_mode = GuiSliderBar(blend_slider, NULL, NULL, active_blend_mode, 0, 6);

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

					// Apply shader
					if (shader_on)
						BeginShaderMode(shader);

					DrawTexturePro(s.image.texture, { 0, 0, (float)s.image.texture.width, (float)-s.image.texture.height }, { kWindowWidth - 850, 50, 800, 450 }, { 0, 0 }, 0.0f, WHITE);
					EndShaderMode();

					DrawText("Preview", kWindowWidth - 425 - MeasureText("PREVIEW", 40) / 2, 250, 40, { 255, 255, 255, 60 });
					break;
				default:
					break;
				}
			default:
				break;
			}
			
			DrawFPS(20, 20);
		}
		EndDrawing();
	}
	CloseWindow();
}