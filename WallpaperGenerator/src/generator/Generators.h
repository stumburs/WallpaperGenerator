#pragma once
#include <vector>
#include <memory>
#include "Generator.h"

class Generators
{
public:
	// Constructor adding all generators to vector
	Generators();
	// Set currently active generator
	void SetActiveGenerator(int generator);
	// Enum containing all possible values for SetActiveGenerator
	enum GeneratorList
	{
		kFlowfield = 0,
		kShapes,
		kVoronoi
	};

	// Functions common to all generators
	void Update();
	void ApplySettings();
	void ResetSettings();
	std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>>& GetUserSettings();
	Texture2D GetImage();
private:
	std::vector<std::unique_ptr<Generator>> generators;
	int active_generator = GeneratorList::kFlowfield;
};