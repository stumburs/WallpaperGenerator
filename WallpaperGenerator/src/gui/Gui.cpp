#include "Gui.h"
#include "../libraries/raygui-3.2/raygui.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
Gui::Gui(int kWindowWidth, int kWindowHeight)
{
	this->kWindowWidth = kWindowWidth;
	this->kWindowHeight = kWindowHeight;

	// Main menu
	create_rect = { (float)kWindowWidth / 2 - 100, (float)kWindowHeight / 2, 200, 50 };
	view_rect = { (float)kWindowWidth / 2 - 100, (float)kWindowHeight / 2 + 75, 200, 50 };
	github_link_rect = { (float)kWindowWidth / 2 - 100, (float)kWindowHeight / 2 + 150 , 200, 50 };
	back_rect_center = { (float)kWindowWidth / 2 - 100, (float)kWindowHeight / 2 + 150 , 200, 50 };

	// Create menu
	flowfield_rect = { (float)kWindowWidth / 2 - 225, (float)kWindowHeight / 2, 200, 50 };
	shapes_rect = { (float)kWindowWidth / 2 + 25, (float)kWindowHeight / 2, 200, 50 };
	voronoi_rect = { (float)kWindowWidth / 2 - 225, (float)kWindowHeight / 2 + 75, 200, 50 };

	// Generate
	preview_rect = { (float)kWindowWidth - 850, 50, 800, 450 };
	shader_checkbox = { (float)kWindowWidth - 850, 525, 40, 40 };
	blend_slider = { (float)kWindowWidth - 650, 525, 400, 40 };

	// Row 1
	update_settings = { (float)kWindowWidth - 800, 585, 160, 40 };
	reset_settings = { (float)kWindowWidth - 600, 585, 160, 40 };
	// Row 2
	save_image = { (float)kWindowWidth - 800, 645, 160, 40 };
	// Row 3
	// Row 4
	back_rect_corner = { (float)kWindowWidth - 800, 765, 160, 40 };

	scroll_pos = { 0 };

	GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
}

void Gui::Draw()
{
	switch (active_menu)
	{
	case Gui::Menu::MAIN:
		MainMenuScreen();
		break;
	case Gui::Menu::CREATE:
		CreateScreen();
		break;
	case Gui::Menu::VIEW:
		ViewScreen();
		break;
	case Gui::Menu::GENERATOR:
		GeneratorScreen(generators);
		break;

	default:
		break;
	}
}

void Gui::Update()
{
	if (active_generator == Gui::ActiveGenerator::NONE)
		return;

	this->preview_texture = generators.GetImage();
	generators.Update();
}

void Gui::MainMenuScreen()
{
	DrawText("Image Generator", kWindowWidth / 2 - MeasureText("Image Generator", 60) / 2 + 4, kWindowHeight / 2 - 196, 60, BLACK);
	DrawText("Image Generator", kWindowWidth / 2 - MeasureText("Image Generator", 60) / 2, kWindowHeight / 2 - 200, 60, RAYWHITE);
	if (GuiButton(create_rect, "Create"))
	{
		active_menu = Menu::CREATE;
	}
	if (GuiButton(view_rect, "View"))
	{
		ViewScreen();
	}

	DrawText(version_number.c_str(), kWindowWidth / 2 - MeasureText(version_number.c_str(), 20) / 2 + 2, kWindowHeight / 2 - 128, 20, BLACK);
	DrawText(version_number.c_str(), kWindowWidth / 2 - MeasureText(version_number.c_str(), 20) / 2, kWindowHeight / 2 - 130, 20, RAYWHITE);

	if (GuiButton(github_link_rect, "GitHub"))
	{
		// Current solution causes heap corruption for unknown reasons.
		OpenURL(github_url.c_str());
	}
}

void Gui::CreateScreen()
{
	DrawText("Create", kWindowWidth / 2 - MeasureText("Create", 60) / 2 + 4, kWindowHeight / 2 - 196, 60, BLACK);
	DrawText("Create", kWindowWidth / 2 - MeasureText("Create", 60) / 2, kWindowHeight / 2 - 200, 60, RAYWHITE);
	if (GuiButton(flowfield_rect, "Flowfield"))
	{
		active_menu = Menu::GENERATOR;
		generators.SetActiveGenerator(Generators::GeneratorList::kFlowfield);
		active_generator = Gui::ActiveGenerator::FLOWFIELD;
		SetTargetFPS(0);
	}
	if (GuiButton(shapes_rect, "Shapes"))
	{
		active_menu = Menu::GENERATOR;
		generators.SetActiveGenerator(Generators::GeneratorList::kShapes);
		active_generator = Gui::ActiveGenerator::SHAPES;
	}
	if (GuiButton(voronoi_rect, "Voronoi"))
	{
		active_menu = Menu::GENERATOR;
		generators.SetActiveGenerator(Generators::GeneratorList::kVoronoi);
		active_generator = Gui::ActiveGenerator::VORONOI;
	}
	if (GuiButton(back_rect_center, "Back"))
	{
		active_menu = Menu::MAIN;
		active_generator = Gui::ActiveGenerator::NONE;
	}
}

void Gui::ViewScreen()
{
	std::string url = GetApplicationDirectory();
	url += "images";
	OpenURL(url.c_str());
}

void Gui::GeneratorScreen(Generators& generators)
{
	Vector2 mouse_pos = GetMousePosition();

	Rectangle scissor_area = GuiScrollPanel({ 50, 45, 655, 800 }, "Settings", { 50, 50, 635, 2000 }, &scroll_pos);
	BeginScissorMode(static_cast<int>(scissor_area.x), static_cast<int>(scissor_area.y), static_cast<int>(scissor_area.width), static_cast<int>(scissor_area.height));

	// Render and get values from sliders
	int index = 0;
	for (std::string& setting_name : generators.GetUserSettings().first)
	{
		Generator::Setting setting = generators.GetUserSettings().second.at(setting_name);

		Rectangle this_rect = { first_setting_rect.x, first_setting_rect.y + index * 60 + scroll_pos.y, first_setting_rect.width, first_setting_rect.height };

		char* text[6]{};
		std::stringstream ss;

		float float_value = 0.0;
		int int_value = 0;

		switch (setting.input_type)
		{
		case Generator::InputType::NONE:
			break;

		case Generator::InputType::GUI_SLIDER_BAR:
			generators.GetUserSettings().second.at(setting_name).value = GuiSliderBar(
				this_rect,
				TextFormat("%0.*f", setting.precision, std::get<float>(setting.value)),
				setting_name.c_str(),
				std::get<float>(setting.value),
				std::get<float>(setting.range.first),
				std::get<float>(setting.range.second)
			);
			break;

		case Generator::InputType::GUI_COLOR_PICKER:
			// Set color picker rectangle height to be 4x larger than the default
			this_rect.height *= 4;
			index += 2;
			generators.GetUserSettings().second.at(setting_name).value = GuiColorPicker(this_rect, "", std::get<Color>(setting.value));
			break;

		case Generator::InputType::GUI_TEXT_BOX:
			float_value = std::get<float>(generators.GetUserSettings().second.at(setting_name).value);

			if (std::isnan(float_value)) // If float is NaN, set text to nothing
				*text = _strdup("");
			else // Else convert float to int and set that as text
			{
				int_value = static_cast<int>(float_value);
				ss << int_value;
				*text = _strdup(ss.str().c_str());
			}

			// Draw text box and update active satus
			if (GuiTextBox(this_rect, *text, 7, generators.GetUserSettings().second.at(setting_name).text_box_editable))
				generators.GetUserSettings().second.at(setting_name).text_box_editable = !generators.GetUserSettings().second.at(setting_name).text_box_editable;

			try // Try-catch ignores non integer characters being input into the text box. Better solution needed
			{
				if (*text && *text[0]) // Convert the non-empty text to a float value
					generators.GetUserSettings().second.at(setting_name).value = std::stof(*text);
				else // Set the value to NaN for empty text
					generators.GetUserSettings().second.at(setting_name).value = std::numeric_limits<float>::quiet_NaN();
			}
			catch (const std::exception&) { std::cout << "Invalid input\n"; }
			break;

		case Generator::InputType::GUI_CHECK_BOX:
			generators.GetUserSettings().second.at(setting_name).value = (float)(int)GuiCheckBox({ this_rect.x, this_rect.y, this_rect.height, this_rect.height }, "Points", std::get<float>(setting.value));
			break;

		default:
			break;
		}

		// Tooltips
		if (CheckCollisionPointRec(mouse_pos, this_rect))
			DrawText(setting.tooltip.c_str(), static_cast<int>(mouse_pos.x - 50), static_cast<int>(mouse_pos.y - 30), 20, DARKGRAY);

		index++;
	}
	EndScissorMode();

	if (GuiButton(save_image, "Save Image"))
	{
		Image img = LoadImageFromTexture(generators.GetImage());
		ImageFlipVertical(&img);
		if (DirectoryExists("images"))
			ExportImage(img, "images/image.png");
		else
			ExportImage(img, "image.png");
	}

	if (GuiButton(update_settings, "Apply settings"))
		generators.ApplySettings();

	if (GuiButton(reset_settings, "Reset settings"))
		generators.ResetSettings();

	DrawRectangle(preview_rect.x - 5, preview_rect.y - 5, preview_rect.width + 10, preview_rect.height + 10, RAYWHITE);
	DrawTexturePro(preview_texture, { 0, 0, (float)preview_texture.width, (float)-preview_texture.height }, preview_rect, { 0, 0 }, 0.0f, WHITE);
	EndShaderMode();

	DrawText("Preview", kWindowWidth - 425 - MeasureText("PREVIEW", 40) / 2, 250, 40, { 255, 255, 255, 60 });

	if (GuiButton(back_rect_corner, "Back"))
	{
		active_menu = Menu::CREATE;
		active_generator = Gui::ActiveGenerator::NONE;
		SetTargetFPS(60);
	}
}
