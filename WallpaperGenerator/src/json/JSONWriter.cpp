#include "JSONWriter.h"
#include <filesystem>

std::string JSONWriter::InputTypeToString(int input_type)
{
	if (input_type == Generator::InputType::GUI_SLIDER_BAR) return "GUI_SLIDER_BAR";
	if (input_type == Generator::InputType::GUI_COLOR_PICKER) return "GUI_COLOR_PICKER";
	if (input_type == Generator::InputType::GUI_TEXT_BOX) return "GUI_TEXT_BOX";
	if (input_type == Generator::InputType::GUI_CHECK_BOX) return "GUI_CHECK_BOX";
	else return "NONE";
}

JSONWriter::json::array_t JSONWriter::ColorToArray(Color color)
{
	return JSONWriter::json::array_t{ color.r, color.g, color.b, color.a };
}

JSONWriter::json::array_t JSONWriter::RangeToJsonArray(std::pair<std::variant<float, Color>, std::variant<float, Color>> range)
{
	json::array_t range_array;

	if (std::holds_alternative<float>(range.first))
		range_array.push_back(std::get<float>(range.first));
	else if (std::holds_alternative<Color>(range.first))
		range_array.push_back(ColorToArray(std::get<Color>(range.first)));

	if (std::holds_alternative<float>(range.second))
		range_array.push_back(std::get<float>(range.second));
	else if (std::holds_alternative<Color>(range.second))
		range_array.push_back(ColorToArray(std::get<Color>(range.second)));

	return range_array;
}

void JSONWriter::WriteSettingsToJson(const std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>>& settings, std::string path)
{
	nlohmann::ordered_json j;

	for (const auto& setting_name : settings.first)
	{
		std::cout << setting_name << '\n';
		const Generator::Setting& setting_data = settings.second.at(setting_name);

		nlohmann::ordered_json j_setting;
		j_setting["input_type"] = InputTypeToString(setting_data.input_type);
		j_setting["name"] = setting_name;
		j_setting["tooltip"] = setting_data.tooltip;

		if (std::holds_alternative<float>(setting_data.value))
			j_setting["value"] = std::get<float>(setting_data.value);
		else if (std::holds_alternative<Color>(setting_data.value))
			j_setting["value"] = ColorToArray(std::get<Color>(setting_data.value));

		j_setting["precision"] = setting_data.precision;
		j_setting["range"] = RangeToJsonArray(setting_data.range);

		j[setting_name] = j_setting;
	}

	std::ofstream output_file(path);
	if (output_file.is_open())
	{
		output_file << j.dump(4);
		output_file.close();
	}
	else
		throw ("Failed to open output file.");
}
