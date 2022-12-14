#pragma once
#include <raylib.h>
#include <raymath.h>

double Map(const double& n, const double& start1, const double& stop1, const double& start2, const double& stop2);

Vector2 Vec2FromAngle(const float& angle);

Vector2 SetMagnitude(const Vector2& v, const float& magnitude);

float SimpleLerp(float a, float b, float f);

int SimpleClamp(int d, int min, int max);