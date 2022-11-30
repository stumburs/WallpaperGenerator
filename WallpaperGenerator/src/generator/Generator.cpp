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
		// Add active blend mode!!!
		Flowfield::Update();
		break;
	case Generators::SHAPES:
		// Add active blend mode!!!
		Shapes::Update(0);
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
		// Add active blend mode!!!
		return Flowfield::GetImage();
		break;
	case Generators::SHAPES:
		// Add active blend mode!!!
		//return Shapes::GetImage();
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
		// Add active blend mode!!!
		Flowfield::Reset(Flowfield::user_values);
		break;
	case Generators::SHAPES:
		// Add active blend mode!!!
		//return Shapes::GetImage();
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
		// Add active blend mode!!!
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
		// Add active blend mode!!!
		Flowfield::user_values.at(key) = value;
		break;
	case Generators::SHAPES:
		// Add active blend mode!!!
		//return Shapes::GetImage();
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
		// Add active blend mode!!!
		return Flowfield::user_values.at(key);
		break;
	case Generators::SHAPES:
		// Add active blend mode!!!
		//return Shapes::GetImage();
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
		// Add active blend mode!!!
		Flowfield::ResetToDefault();
		break;
	case Generators::SHAPES:
		// Add active blend mode!!!
		//return Shapes::GetImage();
		break;
	default:
		break;
	}
}
