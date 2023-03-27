#pragma once
#include <raylib.h>
#include <raymath.h>

double Map(double n, double start1, double stop1, double start2, double stop2);

Vector2 Vec2FromAngle(float angle);

Vector2 SetMagnitude(Vector2 v, float magnitude);

float SimpleLerp(float a, float b, float f);

int SimpleClamp(int d, int min, int max);