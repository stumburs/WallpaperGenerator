#pragma once
#include <raylib.h>
#include <string>
#include <map>
class Generator
{
public:
	enum InputType
	{
		NONE = 0,
		GUI_SLIDER_BAR,
		GUI_COLOR_PICKER,
		GUI_TEXT_BOX
	};

	struct Setting
	{
		std::string name;
		float value;
		int precision;
		std::pair<float, float> range;
		std::string tooltip;
		int input_type;
		std::string string_value = "";
		bool text_box_editable = false;
	};

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void ApplySettings() = 0;
	virtual void ResetSettings() = 0;
	virtual Texture2D GetImage() = 0;
	Color RGBFromString(std::string str);
};