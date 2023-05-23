#include "Gui.h"
#include "../libraries/raygui-3.2/raygui.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
Gui::Gui(int window_width, int window_height)
{
	this->window_width = window_width;
	this->window_height = window_height;

	// Main menu
	orig_create_rect = { (float)window_width / 2 - 100, (float)window_height / 2, 200, 50 };
	create_rect = orig_create_rect;
	orig_view_rect = { (float)window_width / 2 - 100, (float)window_height / 2 + 75, 200, 50 };
	view_rect = orig_view_rect;
	orig_github_link_rect = { (float)window_width / 2 - 100, (float)window_height / 2 + 150 , 200, 50 };
	github_link_rect = orig_github_link_rect;
	orig_back_rect_center = { (float)window_width / 2 - 100, (float)window_height / 2 + 150 , 200, 50 };
	back_rect_center = orig_back_rect_center;

	// Create menu
	flowfield_demo_image = LoadTexture("cfg/flowfield.png");
	voronoi_demo_image = LoadTexture("cfg/voronoi.png");
	shapes_demo_image = LoadTexture("cfg/shapes.png");
	orig_flowfield_rect = { (float)window_width / 2 - 225, (float)window_height / 2, 200, 50 };
	flowfield_rect = orig_flowfield_rect;
	orig_shapes_rect = { (float)window_width / 2 + 25, (float)window_height / 2, 200, 50 };
	shapes_rect = orig_shapes_rect;
	orig_voronoi_rect = { (float)window_width / 2 - 225, (float)window_height / 2 + 75, 200, 50 };
	voronoi_rect = orig_voronoi_rect;

	// Generate
	orig_preview_rect = { (float)window_width - 850, 50, 800, 450 };
	preview_rect = orig_preview_rect;
	//shader_checkbox = { (float)window_width - 850, 525, 40, 40 };
	//blend_slider = { (float)window_width - 650, 525, 400, 40 };

	// Row 1
	orig_update_settings = { (float)window_width - 800, 585, 160, 40 };
	update_settings = orig_update_settings;
	orig_reset_settings = { (float)window_width - 600, 585, 160, 40 };
	reset_settings = orig_reset_settings;
	// Row 2
	orig_save_image = { (float)window_width - 800, 645, 160, 40 };
	save_image = orig_save_image;
	// Row 3
	// Row 4
	orig_back_rect_corner = { (float)window_width - 800, 765, 160, 40 };
	back_rect_corner = orig_save_image;

	scroll_pos = { 0 };

	GuiSetStyle(DEFAULT, TEXT_SIZE, scaled_text_size);
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
	// Update scaling factor
	scale_x = GetScreenWidth() / static_cast<float>(1600);
	scale_y = GetScreenHeight() / static_cast<float>(900);

	// Update font sizes
	float font_scaling_factor = (scale_x + scale_y) / 2.0f;

	// Title text
	scaled_title_font_size = orig_title_font_size * font_scaling_factor;
	// Version subtext
	scaled_version_font_size = orig_version_font_size * font_scaling_factor;

	// Button text size
	scaled_text_size = default_text_size * font_scaling_factor;
	GuiSetStyle(DEFAULT, TEXT_SIZE, scaled_text_size);

	// Update rects
	// Main menu
	create_rect = ScaleRect(orig_create_rect, scale_x, scale_y);
	view_rect = ScaleRect(orig_view_rect, scale_x, scale_y);
	github_link_rect = ScaleRect(orig_github_link_rect, scale_x, scale_y);
	back_rect_center = ScaleRect(orig_back_rect_center, scale_x, scale_y);

	// Create menu
	flowfield_rect = ScaleRect(orig_flowfield_rect, scale_x, scale_y);
	shapes_rect = ScaleRect(orig_shapes_rect, scale_x, scale_y);
	voronoi_rect = ScaleRect(orig_voronoi_rect, scale_x, scale_y);

	// Generator screen
	preview_rect = ScaleRect(orig_preview_rect, scale_x, scale_y);
	save_image = ScaleRect(orig_save_image, scale_x, scale_y);
	update_settings = ScaleRect(orig_update_settings, scale_x, scale_y);
	reset_settings = ScaleRect(orig_reset_settings, scale_x, scale_y);
	back_rect_corner = ScaleRect(orig_back_rect_corner, scale_x, scale_y);

	first_setting_rect = ScaleRect(orig_first_setting_rect, scale_x, scale_y);
	scissor_bounds = ScaleRect(orig_scissor_bounds, scale_x, scale_y);
	scissor_content = ScaleRect(orig_scissor_content, scale_x, scale_y);
	scissor_content.height = 2000;
	scissor_area = ScaleRect(orig_scissor_area, scale_x, scale_y);

	if (active_generator == Gui::ActiveGenerator::NONE)
		return;

	this->preview_texture = generators.GetImage();
	generators.Update();
}

void Gui::MainMenuScreen()
{
	DrawText("Image Generator", round(GetScreenWidth() / static_cast<float>(2) - MeasureText("Image Generator", scaled_title_font_size) / static_cast<float>(2) + 4 * scale_x), round(GetScreenHeight() / static_cast<float>(2) - 196 * scale_y), scaled_title_font_size, BLACK);
	DrawText("Image Generator", round(GetScreenWidth() / 2 - MeasureText("Image Generator", scaled_title_font_size) / 2), round(GetScreenHeight() / static_cast<float>(2) - 200 * scale_y), scaled_title_font_size, RAYWHITE);
	if (GuiButton(create_rect, "Create"))
	{
		active_menu = Menu::CREATE;
	}
	if (GuiButton(view_rect, "View"))
	{
		ViewScreen();
	}

	DrawText(version_number.c_str(), round(GetScreenWidth() / static_cast<float>(2) - MeasureText(version_number.c_str(), scaled_version_font_size) / static_cast<float>(2) + 2 * scale_x), round(GetScreenHeight() / static_cast<float>(2) - 128 * scale_y), scaled_version_font_size, BLACK);
	DrawText(version_number.c_str(), round(GetScreenWidth() / 2 - MeasureText(version_number.c_str(), scaled_version_font_size) / 2), round(GetScreenHeight() / static_cast<float>(2) - 130 * scale_y), scaled_version_font_size, RAYWHITE);

	if (GuiButton(github_link_rect, "GitHub"))
	{
		// Current solution causes heap corruption for unknown reasons.
		OpenURL(github_url.c_str());
	}
}

void Gui::CreateScreen()
{
	DrawText("Create", round(GetScreenWidth() / static_cast<float>(2) - MeasureText("Create", scaled_title_font_size) / static_cast<float>(2) + 4 * scale_x), round(GetScreenHeight() / static_cast<float>(2) - 196 * scale_y), scaled_title_font_size, BLACK);
	DrawText("Create", round(GetScreenWidth() / 2 - MeasureText("Create", scaled_title_font_size) / 2), round(GetScreenHeight() / static_cast<float>(2) - 200 * scale_y), scaled_title_font_size, RAYWHITE);
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

	// Demo images
	float image_scaling_factor = (scale_x + scale_y) / 2.0f;
	Vector2 draw_pos = { GetMousePosition().x - flowfield_demo_image.width / 2 * image_scaling_factor, GetMousePosition().y - flowfield_demo_image.height * image_scaling_factor - 20 * image_scaling_factor };
	if (CheckCollisionPointRec(GetMousePosition(), flowfield_rect))
		DrawTextureEx(flowfield_demo_image, {flowfield_rect.x - ((flowfield_demo_image.width - flowfield_rect.width) / 2) * image_scaling_factor, flowfield_rect.y - (flowfield_demo_image.height + 20) * image_scaling_factor}, 0, image_scaling_factor, WHITE);
	if (CheckCollisionPointRec(GetMousePosition(), voronoi_rect))
		DrawTextureEx(voronoi_demo_image, { voronoi_rect.x - ((voronoi_demo_image.width - voronoi_rect.width) / 2) * image_scaling_factor, voronoi_rect.y - (voronoi_demo_image.height + 20) * image_scaling_factor }, 0, image_scaling_factor, WHITE);
	if (CheckCollisionPointRec(GetMousePosition(), shapes_rect))
		DrawTextureEx(shapes_demo_image, { shapes_rect.x - ((shapes_demo_image.width - shapes_rect.width) / 2) * image_scaling_factor, shapes_rect.y - (shapes_demo_image.height + 20) * image_scaling_factor }, 0, image_scaling_factor, WHITE);
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

	scissor_area = GuiScrollPanel(scissor_bounds, "Settings", scissor_content, &scroll_pos);
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

	DrawText("Preview", GetScreenWidth() - 425 * scale_x - MeasureText("Preview", 40) / 2, 250 * scale_y, 40, {255, 255, 255, 60});

	if (GuiButton(back_rect_corner, "Back"))
	{
		active_menu = Menu::CREATE;
		active_generator = Gui::ActiveGenerator::NONE;
		scroll_pos = { 0 };
		SetTargetFPS(60);
	}
}
