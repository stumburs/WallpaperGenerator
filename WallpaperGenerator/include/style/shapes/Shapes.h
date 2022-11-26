#pragma once
#include "raylib.h"

class Shapes
{
public:
	const int kWindowWidth = 1600;
	const int kWindowHeight = 900;
	int shapes_drawn = 0;
	int shapes_to_draw;

	RenderTexture2D image;

public:
	Shapes();
	void Update();
};

