#include <raylib.h>
#include <iostream>
#include "PerlinNoise.hpp"

const int kWindowWidth = 1280;
const int kWindowHeight = 720;

int main()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(kWindowWidth, kWindowHeight, "Wallpaper Generator");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Update

        // Draw
        BeginDrawing();
        {
            ClearBackground(BLACK);
        }
        EndDrawing();
    }
    CloseWindow();
}