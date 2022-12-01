#pragma once
#include "flowfield/Flowfield.h"
#include "shapes/Shapes.h"

class Generator : public Flowfield, public Shapes
{
public:

	enum Generators
	{
		NONE = 0,
		FLOWFIELD,
		SHAPES,
		AMOUNT = SHAPES
	};

	int active_generator;

	Generator(int generator_type);
	void Update();
	void UpdateSettings();
	void ResetToDefault();
	std::map<std::string, float> GetUserValues();
	void SetValue(std::string key, float value);
	float GetValue(std::string key);
	Texture2D GetImage();
};