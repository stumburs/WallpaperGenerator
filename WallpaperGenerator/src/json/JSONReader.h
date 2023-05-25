#pragma once
#include "json.hpp"
#include "../generator/Generator.h" // For InputType and Setting
#include <raylib.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <variant>

namespace JSONReader
{
	using json = nlohmann::ordered_json;

	// Converts a string to the appropriate Enum value inside InputType
	int StringToInputType(std::string str);

	// Converts a 4 value json array to a color variable containing 4 unsigned chars
	Color ArrayToColor(json::array_t color);

	// Loads settings from json into custom setting data type by providing parsed json data
	std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>> LoadSettingsFromJson(json data);

	// Loads settings from json into custom setting data type by specifying json path
	std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>> LoadSettingsFromJson(std::string path);
}