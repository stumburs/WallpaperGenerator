#include "Shapes.h"
#include "../../Functions.h"
#include <random>

Shapes::Shapes()
{
    // Init texture
    image = LoadRenderTexture(kWindowWidth, kWindowHeight);
    BeginTextureMode(image);
    ClearBackground(WHITE);
    EndTextureMode();

    shapes_to_draw = 100;
}

void Shapes::Update(int active_blend_mode)
{
    // Draw to texture
    while (shapes_drawn < shapes_to_draw)
    {
        Color c = { rand() % 256, rand() % 256, rand() % 256, 20};
        BeginTextureMode(image);
        BeginBlendMode(active_blend_mode);
        DrawPoly({ (float)(rand() % kWindowWidth), (float)(rand() % kWindowHeight) }, rand() % 8 + 2, rand() % 100 + 100, 0.0f, c);
            //DrawCircle(rand() % kWindowWidth, rand() % kWindowHeight, 100.0f, WHITE);
        shapes_drawn++;
        EndBlendMode();
        EndTextureMode();
    }
}