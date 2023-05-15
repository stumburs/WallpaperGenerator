#include "JSONReader.h"
#include <filesystem>

int JSONReader::StringToInputType(std::string str)
{
	if (str == "NONE") return Generator::InputType::NONE;
	if (str == "GUI_SLIDER_BAR") return Generator::InputType::GUI_SLIDER_BAR;
	if (str == "GUI_COLOR_PICKER") return Generator::InputType::GUI_COLOR_PICKER;
	if (str == "GUI_TEXT_BOX") return Generator::InputType::GUI_TEXT_BOX;
	if (str == "GUI_CHECK_BOX") return Generator::InputType::GUI_CHECK_BOX;
	else return Generator::InputType::NONE;
}

Color JSONReader::ArrayToColor(json::array_t color)
{
	return Color{ color[0], color[1], color[2], color[3] };
}

std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>> JSONReader::LoadSettingsFromJson(json data)
{
	std::vector<std::string> insertion_order;
	std::unordered_map<std::string, Generator::Setting> settings;
	for (auto& setting : data)
	{
		Generator::Setting s;
		s.input_type = StringToInputType(setting["input_type"]);
		s.tooltip = setting["tooltip"];

		// Color as value
		if (setting["value"].is_array() && setting["value"].size() == 4)
			s.value = JSONReader::ArrayToColor(setting["value"]);
		// Number as value
		else if (setting["value"].is_number())
			s.value = setting["value"];

		// If precision variable exists
		if (setting.count("precision"))
			s.precision = setting["precision"];

		// If range variable exists
		if (setting.count("range"))
		{
			// Color as range
			if (setting["range"].is_array() && setting["range"].size() == 2 &&
				setting["range"][0].is_array() && setting["range"][0].size() == 4 &&
				setting["range"][1].is_array() && setting["range"][1].size() == 4)
			{
				s.range.first = JSONReader::ArrayToColor(setting["range"][0]);
				s.range.second = JSONReader::ArrayToColor(setting["range"][1]);
			}
			// Numbers as range
			else if (setting["range"].is_array() && setting["range"].size() == 2 &&
				setting["range"][0].is_number() && setting["range"][1].is_number())
			{
				s.range.first = setting["range"][0];
				s.range.second = setting["range"][1];
			}
		}
		settings[setting["name"]] = s;
		insertion_order.push_back(setting["name"]);
		std::cout << setting["name"] << '\n';
	}
	std::cout << '\n';
	return std::make_pair(insertion_order, settings);
}

std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>> JSONReader::LoadSettingsFromJson(std::string path)
{
	std::filesystem::path fs_path = std::filesystem::current_path() / path;
	std::cout << "File path: " << fs_path << std::endl;
	std::cout << "Got path: " << path << '\n';
	std::ifstream file(path);
	std::cout << "file opened succesfully: " << std::boolalpha << file.is_open() << '\n';
	json j = json::parse(file);
	std::cout << "parsed file\n";
	return LoadSettingsFromJson(j);
}
