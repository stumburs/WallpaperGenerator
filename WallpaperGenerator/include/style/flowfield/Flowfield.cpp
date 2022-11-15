#include "Flowfield.h"
#include "../../PerlinNoise.hpp"
#include "../../Functions.h"
Flowfield::Flowfield()
{
	render_width = kWindowWidth / scale + 1;
	render_height = kWindowHeight / scale + 1;

    // Perlin noise setup
    seed = 69420u;
    perlin.reseed(seed);

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
            float angle = (float)Map(perlin.octave3D_01((x * x_mult), (y * y_mult), (noise_height * z_mult), noise_detail), 0, 1, 0, 359);
            Vector2 vec = Vec2FromAngle(angle);
            vec = SetMagnitude(vec, 1);
            flowfield[x].push_back(vec);
        }
    }


    // Init texture
    image = LoadRenderTexture(kWindowWidth, kWindowHeight);
    BeginTextureMode(image);
    ClearBackground(BLACK);
    EndTextureMode();

    z = noise_height;
}

void Flowfield::Update()
{
    // Flowfield
    for (int x = 0; x < flowfield.size(); x++)
    {
        for (int y = 0; y < flowfield[x].size(); y++)
        {
            // Calculate each vector
            double angle = Map(perlin.octave3D_01((x * x_mult), (y * y_mult), (z * z_mult), noise_detail), 0, 1, 0, 359);
            Vector2 vec = Vec2FromAngle((float)angle);
            vec = SetMagnitude(vec, flowfield_strength);
            flowfield[x][y] = vec;
        }
    }

    // Update particles
    for (int i = 0; i < particles.size(); i++)
    {
        particles[i].Update(flowfield, particle_speed, kWindowWidth, kWindowHeight, scale);
    }

    // Draw to texture
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
    z += 0.06f;
}