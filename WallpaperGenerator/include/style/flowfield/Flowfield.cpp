#include "Flowfield.h"
#include "../../PerlinNoise.hpp"
#include "../../Functions.h"
Flowfield::Flowfield()
{
	render_width = window_width / scale + 1;
	render_height = window_height / scale + 1;

    // Perlin noise setup
    seed = 69420u;
    perlin.reseed(seed);

    InitParticles();

    // Init flowfield
    InitFlowfield();

    // Reset / Init Texture
    Reset(BLACK);
}

void Flowfield::Update(int active_blend_mode)
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
        particles[i].Update(flowfield, particle_speed, window_width, window_height, scale);
    }

    // Draw to texture
    BeginTextureMode(image);
    {
        BeginBlendMode(active_blend_mode);
        // Draw Particles
        for (int i = 0; i < particles.size(); i++)
        {
            particles[i].DrawPixel({ 255,
                                        (unsigned char)Map(particles[i].pos.x, 0, window_width, 0, 255),
                                        (unsigned char)Map(particles[i].pos.y, 0, window_height, 0, 255),
                                        particle_strength });
        }
        EndBlendMode();
    }
    EndTextureMode();

    // Move through noise
    z += 0.06f;
}

// Init / Reset image
void Flowfield::Reset(Color background_color)
{
    image = LoadRenderTexture(window_width, window_height);
    BeginTextureMode(image);
    ClearBackground(background_color);
    EndTextureMode();
    z = noise_height;
}

void Flowfield::SetValues(std::map<std::string, float> user_values)
{
    window_width = (int)user_values["window_width"];
    window_height = (int)user_values["window_height"];
    scale = (int)user_values["scale"];
    seed = (unsigned char)user_values["seed"];
    flowfield_strength = user_values["flowfield_strength"];
    particle_count = (int)user_values["particle_count"];
    particle_speed = user_values["particle_speed"];
    particle_size = user_values["particle_size"];
    particle_strength = (unsigned char)user_values["particle_strength"];
    noise_height = user_values["noise_height"];
    noise_detail = (int)user_values["noise_detail"];
    x_mult = user_values["x_mult"];
    y_mult = user_values["y_mult"];
    z_mult = user_values["z_mult"];
    z = user_values["z"];

    InitParticles();
    Reset(BLACK);
}

void Flowfield::InitParticles() {
    particles.clear();
    for (int i = 0; i < particle_count; i++)
    {
        Particle p;
        p.pos = { (float)GetRandomValue(0, window_width), (float)GetRandomValue(0, window_height) };
        particles.push_back(p);
    }
}

void Flowfield::InitFlowfield()
{
    flowfield.clear();
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
}