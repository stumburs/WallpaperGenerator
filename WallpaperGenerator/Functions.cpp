#include "Functions.h"

double Map(double n, double start1, double stop1, double start2, double stop2)
{
    return ((n - start1) / (stop1 - start1)) * (stop2 - start2) + start2;
}

Vector2 Vec2FromAngle(float angle)
{
    return Vector2{ cosf(angle * DEG2RAD), sinf(angle * DEG2RAD) };
}

Vector2 SetMagnitude(Vector2 v, float magnitude)
{
    Vector2 new_vec = Vector2Normalize(v);
    new_vec.x = v.x * magnitude;
    new_vec.y = v.y * magnitude;
    return new_vec;
}