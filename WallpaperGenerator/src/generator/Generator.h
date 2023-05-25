#pragma once
#include <raylib.h>
#include <string>
#include <map>
#include <vector>
#include <variant>
#include <unordered_map>
class Generator
{
public:
	enum InputType
	{
		NONE = 0,
		GUI_SLIDER_BAR,
		GUI_COLOR_PICKER,
		GUI_TEXT_BOX,
		GUI_CHECK_BOX
	};

	struct Setting
	{
		int input_type = Generator::InputType::NONE;
		std::string tooltip = "";
		std::variant<float, Color> value = 0.0f;
		int precision = 0;
		std::pair<std::variant<float, Color>, std::variant<float, Color>> range = { 0.0f, 0.0f };
		bool text_box_editable = false;
	};

	std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>> default_settings;
	std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>> user_settings;

	virtual void Update() = 0;
	virtual void ApplySettings() = 0;
	virtual void ResetSettings() = 0;
	virtual Texture2D GetImage() = 0;
	virtual std::pair<std::vector<std::string>, std::unordered_map<std::string, Generator::Setting>>& GetUserSettings() = 0;
	virtual void InitializeDefaultVariablesFromSettings() = 0;
	virtual void SaveSettingsToJson() = 0;
	Color RGBFromString(const std::string& str);
};