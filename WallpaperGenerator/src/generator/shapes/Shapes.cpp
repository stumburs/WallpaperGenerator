#include "Shapes.h"
#include "../../Functions.h"
#include <random>

Shapes::Shapes()
{
    Shapes::Reset();

}

Texture2D Shapes::GetImage()
{
    return image.texture;
}

void Shapes::Update()
{
    // Draw to texture
    while (shapes_drawn < shapes_to_draw)
    {
        Color c = { rand() % 256, rand() % 256, rand() % 256, 20};
        BeginTextureMode(image);
        BeginBlendMode(active_blend_mode);
        DrawPoly({ (float)(rand() % window_width), (float)(rand() % window_height) }, rand() % 8 + 2, rand() % 100 + 100, 0.0f, c);
            //DrawCircle(rand() % kWindowWidth, rand() % kWindowHeight, 100.0f, WHITE);
        shapes_drawn++;
        EndBlendMode();
        EndTextureMode();
    }
}

void Shapes::Reset()
{
    window_width = user_values["window_width"];
    window_height = user_values["window_height"];
    shapes_to_draw = user_values["shapes_to_draw"];
    active_blend_mode = user_values["active_blend_mode"];
    shapes_drawn = 0;
    // Init texture
    UnloadRenderTexture(image);
    image = LoadRenderTexture(window_width, window_height);
    BeginTextureMode(image);
    ClearBackground(BLACK);
    EndTextureMode();
}