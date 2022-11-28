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
		Flowfield::Update(0);
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