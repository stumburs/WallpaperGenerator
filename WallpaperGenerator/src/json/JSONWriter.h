#pragma once
#include "json.hpp"
#include "../generator/Generator.h" // For InputType and Setting
#include <raylib.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <variant>

namespace JSONWriter
{
	using json = nlohmann::ordered_json;

	// Converts the integer value stored in the InputType enum to the appropriate string value
	std::string InputTypeToString(int input_type);

	// Converts a RAYLIB_H::Color to a json array
	json::array_t ColorToArray(Color color);

	// Converts setting range into a json array
	json::array_t RangeToJsonArray(std::pair<std::variant<float, Color>, std::variant<float, Color>> range);

	// Writes user settings to a json file
	void WriteSettingsToJson(const std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>>& settings, std::string path);
}