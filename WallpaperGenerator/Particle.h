#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>

struct Particle
{
    Vector2 pos; // Position
    Vector2 vel; // Velocity
    Vector2 acc; // Acceleration

    void Update(const std::vector<std::vector<Vector2>>& flowfield, float max_speed, int WIDTH, int HEIGHT, int SCALE);

    void Follow(const std::vector<std::vector<Vector2>>& flowfield, int SCALE);

    void ApplyForce(Vector2 force);

    void WrapAroundEdges(int WIDTH, int HEIGHT);

    void DrawCircle(float size, Color color);

    void DrawPixel(Color color);
};