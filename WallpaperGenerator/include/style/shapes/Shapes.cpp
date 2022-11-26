#include "Shapes.h"
#include "../../Functions.h"
#include <random>

Shapes::Shapes()
{
    // Init texture
    image = LoadRenderTexture(kWindowWidth, kWindowHeight);
    BeginTextureMode(image);
    ClearBackground(BLACK);
    EndTextureMode();
}

void Shapes::Update()
{
    // Draw to texture
    if (shapes_drawn < 10)
    {
        BeginTextureMode(image);
        for (int i = 0; i < 10; i++)
        {
            DrawCircle(rand() % kWindowWidth, rand() % kWindowHeight, 100.0f, WHITE);
            shapes_drawn++;
        }
        EndTextureMode();
    }
}