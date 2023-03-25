#pragma once
#include "Particle.h"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include "../../Functions.h"

void Particle::Update(const std::vector<std::vector<Vector2>>& flowfield, const float& max_speed, const int& WIDTH, const int& HEIGHT, const int& SCALE)
{
	vel = Vector2Add(vel, acc);
	vel = Vector2Normalize(vel);
	pos = Vector2Add(pos, Vector2Scale(vel, max_speed));
	WrapAroundEdges(WIDTH, HEIGHT);
	acc = { 0 };
	Follow(flowfield, SCALE);
}

void Particle::Follow(const std::vector<std::vector<Vector2>>& flowfield, const int& SCALE)
{
	float x = floorf(pos.x / SCALE);
	float y = floorf(pos.y / SCALE);
	Vector2 force = flowfield[x][y];
	ApplyForce(force);
}

void Particle::ApplyForce(const Vector2& force)
{
	acc = Vector2Add(acc, force);
}

void Particle::WrapAroundEdges(const int& WIDTH, const int& HEIGHT)
{
	if (pos.x > (float)WIDTH) pos.x = 0;
	if (pos.x < 0) pos.x = (float)WIDTH;
	if (pos.y > (float)HEIGHT) pos.y = 0;
	if (pos.y < 0) pos.y = (float)HEIGHT;
}

void Particle::DrawCircle(const float& size, const Color& color)
{
	RAYLIB_H::DrawCircle(pos.x, pos.y, size, color);
}

void Particle::DrawPixel(const Color& color)
{
	RAYLIB_H::DrawPixel(pos.x, pos.y, color);
}

Color Particle::GetColorAtPos(const std::vector<std::vector<Color>>& vec)
{
	int x = SimpleClamp((int)pos.x, 0, (int)vec.size() - 1);
	int y = SimpleClamp((int)pos.y, 0, (int)vec[x].size() - 1);
	return vec.at(x).at(y);
}