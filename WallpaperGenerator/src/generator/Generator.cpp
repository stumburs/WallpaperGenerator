#include "Generator.h"

Generator::Generator(int generator_type) : Flowfield(), Shapes()
{
	active_generator = generator_type;
}

void Generator::Update()
{
	switch (active_generator)
	{
	case Generators::FLOWFIELD:
		Flowfield::Update();
		break;
	case Generators::SHAPES:
		Shapes::Update();
		break;
	default:
		break;
	}
}

Texture2D Generator::GetImage()
{
	switch (active_generator)
	{
	case Generators::FLOWFIELD:
		return Flowfield::GetImage();
		break;
	case Generators::SHAPES:
		return Shapes::GetImage();
		break;
	default:
		break;
	}
}

void Generator::UpdateSettings()
{
	switch (active_generator)
	{
	case Generators::FLOWFIELD:
		Flowfield::Reset();
		break;
	case Generators::SHAPES:
		Shapes::Reset();
		break;
	default:
		break;
	}
}

std::map<std::string, float> Generator::GetUserValues()
{
	switch (active_generator)
	{
	case Generators::FLOWFIELD:
		return Flowfield::GetUserValues();
		break;
	case Generators::SHAPES:
		// Add active blend mode!!!
		//return Shapes::GetImage();
		break;
	default:
		break;
	}
}

void Generator::SetValue(std::string key, float value)
{
	switch (active_generator)
	{
	case Generators::FLOWFIELD:
		Flowfield::user_values.at(key) = value;
		break;
	case Generators::SHAPES:
		Shapes::user_values.at(key) = value;
		break;
	default:
		break;
	}
}

float Generator::GetValue(std::string key)
{
	switch (active_generator)
	{
	case Generators::FLOWFIELD:
		return Flowfield::user_values.at(key);
		break;
	case Generators::SHAPES:
		return Shapes::user_values.at(key);
		break;
	default:
		break;
	}
}

void Generator::ResetToDefault()
{
	switch (active_generator)
	{
	case Generators::FLOWFIELD:
		Flowfield::user_values = Flowfield::default_values;
		break;
	case Generators::SHAPES:
		Shapes::user_values = Shapes::default_values;
		break;
	default:
		break;
	}
}
