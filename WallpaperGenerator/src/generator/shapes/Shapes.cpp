#include "Shapes.h"
#include "../../Functions.h"
#include <random>

Shapes::Shapes()
{
    Shapes::Init();
}

void Shapes::Init()
{
    window_width = user_settings[0].value;
    window_height = user_settings[1].value;
    shapes_amount = user_settings[2].value;
    shape_alpha = user_settings[3].value;
    seed = user_settings[4].value;
    blend_mode = user_settings[5].value;
    shapes_drawn = 0;

    // Init texture
    UnloadRenderTexture(image);
    image = LoadRenderTexture(window_width, window_height);
    BeginTextureMode(image);
    ClearBackground(BLACK);
    EndTextureMode();
}

void Shapes::Update()
{
    // Draw to texture
    while (shapes_drawn < shapes_amount)
    {
        Color c = { rand() % 256, rand() % 256, rand() % 256, shape_alpha };
        BeginTextureMode(image);
        BeginBlendMode(blend_mode);
        DrawPoly({ (float)(rand() % window_width), (float)(rand() % window_height) }, rand() % 8 + 2, rand() % 100 + 100, 0.0f, c);
        shapes_drawn++;
        EndBlendMode();
        EndTextureMode();
    }
}

void Shapes::ApplySettings()
{
    window_width = user_settings[0].value;
    window_height = user_settings[1].value;
    shapes_amount = user_settings[2].value;
    shape_alpha = user_settings[3].value;
    seed = user_settings[4].value;
    blend_mode = user_settings[5].value;
    shapes_drawn = 0;

    // Init texture
    UnloadRenderTexture(image);
    image = LoadRenderTexture(window_width, window_height);
    BeginTextureMode(image);
    ClearBackground(BLACK);
    EndTextureMode();
}

void Shapes::ResetSettings()
{
    user_settings = default_settings;
}

Texture2D Shapes::GetImage()
{
    return image.texture;
}
