#include "Generators.h"
#include "flowfield/Flowfield.h"
#include "shapes/Shapes.h"
#include "voronoi/Voronoi.h"

Generators::Generators()
{
	generators.emplace_back(std::make_unique<Flowfield>());
	generators.emplace_back(std::make_unique<Shapes>());
	generators.emplace_back(std::make_unique<Voronoi>());
}

void Generators::SetActiveGenerator(int generator) { this->active_generator = generator; }

void Generators::Update() { generators[active_generator]->Update(); }
void Generators::ApplySettings() { generators[active_generator]->ApplySettings(); }
void Generators::ResetSettings() { generators[active_generator]->ResetSettings(); }
std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>>& Generators::GetUserSettings() { return generators[active_generator]->GetUserSettings(); }
Texture2D Generators::GetImage() { return generators[active_generator]->GetImage(); }