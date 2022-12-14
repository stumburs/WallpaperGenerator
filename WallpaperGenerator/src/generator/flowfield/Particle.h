#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>

struct Particle
{
    Vector2 pos; // Position
    Vector2 vel; // Velocity
    Vector2 acc; // Acceleration

    void Update(const std::vector<std::vector<Vector2>>& flowfield, const float& max_speed, const int& WIDTH, const int &HEIGHT, const int &SCALE);

    void Follow(const std::vector<std::vector<Vector2>>& flowfield, const int &SCALE);

    void ApplyForce(const Vector2 &force);

    void WrapAroundEdges(const int &WIDTH, const int &HEIGHT);

    void DrawCircle(const float &size, const Color &color);

    void DrawPixel(const Color& color);

    Color GetColorAtPos(const std::vector<std::vector<Color>>& vec);
};