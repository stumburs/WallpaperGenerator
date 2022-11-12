#include "Functions.h"

double Map(const double& n, const double& start1, const double& stop1, const double& start2, const double& stop2)
{
    return ((n - start1) / (stop1 - start1)) * (stop2 - start2) + start2;
}

Vector2 Vec2FromAngle(const float &angle)
{
    return Vector2{ cosf(angle * DEG2RAD), sinf(angle * DEG2RAD) };
}

Vector2 SetMagnitude(const Vector2 &v, const float &magnitude)
{
    Vector2 new_vec = Vector2Normalize(v);
    new_vec.x = v.x * magnitude;
    new_vec.y = v.y * magnitude;
    return new_vec;
}