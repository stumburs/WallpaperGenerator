#pragma once
#include "flowfield/Flowfield.h"
#include "shapes/Shapes.h"

class Generator : public Flowfield, public Shapes
{
public:

	enum Generators
	{
		FLOWFIELD = 0,
		SHAPES
	};

	int active_generator;

	Generator(int generator_type);
	void Update();
	Texture2D GetImage();
};