#include <raylib.h>
#include <iostream>
#include <vector>
#include "Particle.h"
#include "Functions.h"
#include "PerlinNoise.hpp"

const int kWindowWidth = 1600;
const int kWindowHeight = 900;

int scale = 20;

std::vector<std::vector<Vector2>> flowfield;

float flowfield_strength = 0.01f;
int particle_count = 5000;
float particle_speed = 1.0f;
float particle_size = 1.0f;
unsigned char particle_strength = 1;
std::vector<Particle> particles;

float noise_height = 0;
int noise_detail = 6;
float x_mult = 0.02;
float y_mult = 0.02;
float z_mult = 0.02;
float z = 0;

bool to_shader = false;

int main()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(kWindowWidth, kWindowHeight, "Wallpaper Generator");
    SetTargetFPS(0);

	Shader shader = LoadShader(0, "PIXELIZER.fs");
	float resolution[2] = { kWindowWidth, kWindowHeight };
	//SetShaderValue(shader, GetShaderLocation(shader, "resolution"), resolution, SHADER_UNIFORM_VEC2);

	
	std::cout << GetShaderLocation(shader, "texture0") << "\n\n\n\n";

    int render_width = kWindowWidth / scale + 1;
    int render_height = kWindowHeight / scale + 1;

    // Perlin noise setup
    const siv::PerlinNoise::seed_type seed = 69420u;
    const siv::PerlinNoise perlin{ seed };

    // Init particles
    for (int i = 0; i < particle_count; i++)
    {
        Particle p;
        p.pos = { (float)GetRandomValue(0, kWindowWidth), (float)GetRandomValue(0, kWindowHeight) };
        particles.push_back(p);
    }

    // Init flowfield
    for (int x = 0; x < render_width; x++)
    {
        flowfield.push_back(std::vector<Vector2>());
        for (int y = 0; y < render_height; y++)
        {
            // Calculate each vector
            float angle = Map(perlin.octave3D_01((x * x_mult), (y * y_mult), (noise_height * z_mult), noise_detail), 0, 1, 0, 359);
            Vector2 vec = Vec2FromAngle(angle);
            vec = SetMagnitude(vec, 1);
            flowfield[x].push_back(vec);
        }
    }

	RenderTexture2D image = LoadRenderTexture(kWindowWidth, kWindowHeight);
	BeginTextureMode(image);
	ClearBackground(BLACK);
	EndTextureMode();

    // Set noise height
    z = noise_height;

    bool background_cleared = false;

	while (!WindowShouldClose())
	{
		// Update
		{

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				to_shader = !to_shader;
			}

			// Flowfield
			for (int x = 0; x < flowfield.size(); x++)
			{
				for (int y = 0; y < flowfield[x].size(); y++)
				{
					// Calculate each vector
					double angle = Map(perlin.octave3D_01((x * x_mult), (y * y_mult), (z * z_mult), noise_detail), 0, 1, 0, 359);
					Vector2 vec = Vec2FromAngle(angle);
					vec = SetMagnitude(vec, flowfield_strength);
					flowfield[x][y] = vec;
				}

			}

			// Update particles
			for (int i = 0; i < particles.size(); i++)
			{
				particles[i].Update(flowfield, particle_speed, kWindowWidth, kWindowHeight, scale);
			}

			BeginTextureMode(image);
			{

				// Draw Particles
				for (int i = 0; i < particles.size(); i++)
				{
					particles[i].DrawPixel({ 255,
												(unsigned char)Map(particles[i].pos.x, 0, kWindowWidth, 0, 255),
												(unsigned char)Map(particles[i].pos.y, 0, kWindowHeight, 0, 255),
												particle_strength });
				}

			}
			EndTextureMode();



			// Move through noise
			z += 0.06;

		}

		// Draw
		BeginDrawing();
		{
			ClearBackground(BLACK);
			BeginBlendMode(BLEND_ADD_COLORS);

			//DrawCircle(400, 300, 20, { 255, 255, 0, 255 });

			//SetShaderValueTexture(shader, GetShaderLocation(shader, "texture0"), image.texture);

			if (to_shader)
			{
				BeginShaderMode(shader);

			}

			DrawTextureRec(image.texture, { 0, 0, (float)image.texture.width, (float)-image.texture.height }, { 0, 0 }, WHITE);
			EndShaderMode();

			EndBlendMode();
			DrawFPS(20, 20);

			if (to_shader)
			{
				DrawText("YES", 20, 40, 20, GREEN);

			}

		}
		EndDrawing();
	}
    CloseWindow();
}