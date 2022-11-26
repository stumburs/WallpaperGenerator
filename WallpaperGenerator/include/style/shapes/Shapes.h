#pragma once
#include "raylib.h"

class Shapes
{
public:
	const int kWindowWidth = 1600;
	const int kWindowHeight = 900;
	int shapes_drawn = 0;

	RenderTexture2D image;

public:
	Shapes();
	void Update();
};

