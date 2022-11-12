#include <raylib.h>
#include <iostream>

int main()
{
    InitWindow(1280, 720, "Wallpaper Generator");
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