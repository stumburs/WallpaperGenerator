#include "Shapes.h"
#include <random>
#include "../../Functions.h"

Shapes::Shapes()
{
    Shapes::Init();
}

void Shapes::Init()
{
    try
    {
        window_width = std::stoi(user_settings[0].string_value);
        window_height = std::stoi(user_settings[1].string_value);
    }
    catch (const std::exception&) {}
    try
    {
        seed = std::stoi(user_settings[2].string_value);
    }
    catch (const std::exception&) {}
    try
    {
        shapes_amount = std::stoi(user_settings[3].string_value);
    }
    catch (const std::exception&) {}
    background_color = { (unsigned char)std::stoi(user_settings[4].string_value.substr(0, 3)), (unsigned char)std::stoi(user_settings[4].string_value.substr(3, 3)), (unsigned char)std::stoi(user_settings[4].string_value.substr(6, 3)), 255 };
    shape_alpha = user_settings[5].value;
    blend_mode = user_settings[6].value;
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
    srand(seed);
    // Draw to texture
    while (shapes_drawn < shapes_amount)
    {
        Color c = { rand() % 256, rand() % 256, rand() % 256, shape_alpha };
        BeginTextureMode(image);
        BeginBlendMode(blend_mode);
        DrawPoly({ (float)(rand() % window_width), (float)(rand() % window_height) }, rand() % 8 + 2, rand() % 100 + 100, rand() % 100 * 0.1, c);
        shapes_drawn++;
        EndBlendMode();
        EndTextureMode();
    }
}

void Shapes::ApplySettings()
{
    try
    {
        window_width = std::stoi(user_settings[0].string_value);
        window_height = std::stoi(user_settings[1].string_value);
    }
    catch (const std::exception&) {}
    try
    {
        seed = std::stoi(user_settings[2].string_value);
    }
    catch (const std::exception&) {}
    try
    {
        shapes_amount = std::stoi(user_settings[3].string_value);
    }
    catch (const std::exception&) {}
    background_color = { (unsigned char)std::stoi(user_settings[4].string_value.substr(0, 3)), (unsigned char)std::stoi(user_settings[4].string_value.substr(3, 3)), (unsigned char)std::stoi(user_settings[4].string_value.substr(6, 3)), 255 };
    shape_alpha = user_settings[5].value;
    blend_mode = user_settings[6].value;
    shapes_drawn = 0;

    // Init texture
    UnloadRenderTexture(image);
    image = LoadRenderTexture(window_width, window_height);
    BeginTextureMode(image);
    ClearBackground(background_color);
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
