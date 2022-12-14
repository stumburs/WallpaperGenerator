#include "Gui.h"
#include "raygui.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
Gui::Gui(int kWindowWidth, int kWindowHeight)
{
	this->kWindowWidth = kWindowWidth;
	this->kWindowHeight = kWindowHeight;

	// Main menu
	create_rect = { (float)kWindowWidth / 2 - 450, (float)kWindowHeight / 2 - 50, 400, 100 };
	view_rect = { (float)kWindowWidth / 2 + 50, (float)kWindowHeight / 2 - 50, 400, 100 };
	github_link_rect = { (float)kWindowWidth - 120, (float)kWindowHeight - 60 , 100, 40 };
	back_rect_center = { (float)kWindowWidth / 2 - 100, (float)kWindowHeight / 2 + 100, 200, 100 };
	back_rect_corner = { (float)kWindowWidth - 120, (float)kWindowHeight - 60 , 100, 40 };

	// Create menu
	flowfield_rect = { (float)kWindowWidth / 2 - 450, (float)kWindowHeight / 2 - 50, 400, 100 };
	shapes_rect = { (float)kWindowWidth / 2 + 50, (float)kWindowHeight / 2 - 50, 400, 100 };

	// Generate
	preview_rect = { (float)kWindowWidth - 850, 50, 800, 450 };
	shader_checkbox = { (float)kWindowWidth - 850, 525, 40, 40 };
	blend_slider = { (float)kWindowWidth - 650, 525, 400, 40 };
	save_image = { (float)kWindowWidth - 800, 585, 160, 40 };
	update_settings = { (float)kWindowWidth - 800, 645, 160, 40 };
	reset_settings = { (float)kWindowWidth - 600, 645, 160, 40 };

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

		switch (active_generator)
		{
		case Gui::ActiveGenerator::NONE:
			break;
		case Gui::ActiveGenerator::FLOWFIELD:
			FlowfieldScreen();
			break;
		case Gui::ActiveGenerator::SHAPES:
			ShapesScreen();
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}
}

void Gui::Update()
{
	if (active_generator == Gui::ActiveGenerator::NONE)
		return;

	switch (active_generator)
	{
	case Gui::ActiveGenerator::FLOWFIELD:
		this->preview_texture = f.GetImage();
		f.Update();
		break;
	case Gui::ActiveGenerator::SHAPES:
		this->preview_texture = s.GetImage();
		s.Update();
		break;
	default:
		break;
	}
}

void Gui::MainMenuScreen()
{
	if (GuiButton(create_rect, "Create"))
	{
		active_menu = Menu::CREATE;
	}
	if (GuiButton(view_rect, "View"))
	{
		ViewScreen();
	}

	DrawText(version_number.c_str(), kWindowWidth - MeasureText(version_number.c_str(), 20) - 160, kWindowHeight - 50, 20, BLACK);

	if (GuiButton(github_link_rect, "GitHub"))
	{
		// Current solution causes heap corruption for unknown reasons.
		OpenURL(github_url.c_str());
	}
}

void Gui::CreateScreen()
{
	if (GuiButton(flowfield_rect, "Flowfield"))
	{
		active_menu = Menu::GENERATOR;
		active_generator = Gui::ActiveGenerator::FLOWFIELD;
		SetTargetFPS(0);
	}
	if (GuiButton(shapes_rect, "Shapes"))
	{
		active_menu = Menu::GENERATOR;
		active_generator = Gui::ActiveGenerator::SHAPES;
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

void Gui::FlowfieldScreen()
{
	Vector2 mouse_pos = GetMousePosition();

	Rectangle scissor_area = GuiScrollPanel({ 50, 50, 650, 800 }, NULL, { 50, 50, 635, 2000 }, &scroll_pos);
	BeginScissorMode(scissor_area.x, scissor_area.y, scissor_area.width, scissor_area.height);

	// Render and get values from sliders
	int index = 0;
	for (auto& setting : f.user_settings)
	{
		Rectangle this_rect = { first_setting_rect.x, first_setting_rect.y + index * 60 + scroll_pos.y, first_setting_rect.width, first_setting_rect.height };
		float value{};
		int color_as_int{};
		Color c{};
		std::string return_value = "";
		std::stringstream ss(return_value);

		switch (setting.input_type)
		{
		case Flowfield::InputType::NONE:
			break;
		case Flowfield::InputType::GUI_SLIDER_BAR:
			value = GuiSliderBar(
				this_rect,
				TextFormat("%0.*f", setting.precision, setting.value),
				setting.name.c_str(),
				setting.value,
				setting.range.first,
				setting.range.second
			);
			setting.value = value;
			break;
		case Flowfield::InputType::GUI_COLOR_PICKER:
			this_rect.height *= 4;
			index += 2;
			c = { (unsigned char)std::stoi(setting.string_value.substr(0, 3)), (unsigned char)std::stoi(setting.string_value.substr(3, 3)), (unsigned char)std::stoi(setting.string_value.substr(6, 3)) };
			c = GuiColorPicker(this_rect, "", c);
			ss << std::setfill('0') << std::setw(3) << (int)c.r << std::setfill('0') << std::setw(3) << (int)c.g << std::setfill('0') << std::setw(3) << (int)c.b;
			setting.string_value = ss.str();
			break;
		default:
			break;
		}
		
		index++;
	}

	// Display tooltips
	int i = 0;
	for (const auto& setting : f.user_settings)
	{
		Rectangle rect = { first_setting_rect.x, first_setting_rect.y + i * 60 + scroll_pos.y, first_setting_rect.width, first_setting_rect.height };
		if (CheckCollisionPointRec(mouse_pos, rect))
			DrawText(setting.tooltip.c_str(), mouse_pos.x, mouse_pos.y - 30, 20, DARKGRAY);
		i++;
	}
	EndScissorMode();

	if (GuiButton(save_image, "Save Image"))
	{
		Image img = LoadImageFromTexture(f.GetImage());
		ImageFlipVertical(&img);
		if (DirectoryExists("images"))
			ExportImage(img, "images/image.png");
		else
			ExportImage(img, "image.png");
	}

	if (GuiButton(update_settings, "Apply settings"))
		f.ApplySettings();

	if (GuiButton(reset_settings, "Reset settings"))
		f.ResetSettings();

	// Apply shader
	//if (shader_on)
	//	BeginShaderMode(shader);
	DrawTexturePro(preview_texture, { 0, 0, (float)preview_texture.width, (float)-preview_texture.height }, preview_rect, { 0, 0 }, 0.0f, WHITE);
	//EndShaderMode();

	DrawText("Preview", kWindowWidth - 425 - MeasureText("PREVIEW", 40) / 2, 250, 40, { 255, 255, 255, 60 });

	if (GuiButton(back_rect_corner, "Back"))
	{
		active_menu = Menu::CREATE;
		active_generator = Gui::ActiveGenerator::NONE;
		SetTargetFPS(60);
	}
}

void Gui::ShapesScreen()
{
	// Preview Box
	DrawRectangleRec(preview_rect, WHITE);

	Vector2 mouse_pos = GetMousePosition();

	Rectangle scissor_area = GuiScrollPanel({ 50, 50, 650, 800 }, NULL, { 50, 50, 635, (float)s.user_settings.size() * 63 }, &scroll_pos);
	BeginScissorMode(scissor_area.x, scissor_area.y, scissor_area.width, scissor_area.height);

	// Render and get values from sliders
	int index = 0;
	for (auto& setting : s.user_settings)
	{
		Rectangle this_rect = { first_setting_rect.x, first_setting_rect.y + index * 60 + scroll_pos.y, first_setting_rect.width, first_setting_rect.height };

		float value = GuiSliderBar(
			this_rect,
			TextFormat("%0.*f", setting.precision, setting.value),
			setting.name.c_str(),
			setting.value,
			setting.range.first,
			setting.range.second
		);
		setting.value = value;

		index++;
	}

	// Display tooltips
	int i = 0;
	for (const auto& setting : s.user_settings)
	{
		Rectangle rect = { first_setting_rect.x, first_setting_rect.y + i * 60 + scroll_pos.y, first_setting_rect.width, first_setting_rect.height };
		if (CheckCollisionPointRec(mouse_pos, rect))
			DrawText(setting.tooltip.c_str(), mouse_pos.x, mouse_pos.y - 30, 20, DARKGRAY);
		i++;
	}
	EndScissorMode();

	if (GuiButton(save_image, "Save Image"))
	{
		Image img = LoadImageFromTexture(s.GetImage());
		ImageFlipVertical(&img);
		if (DirectoryExists("images"))
			ExportImage(img, "images/image.png");
		else
			ExportImage(img, "image.png");
	}

	if (GuiButton(update_settings, "Apply settings"))
		s.ApplySettings();

	if (GuiButton(reset_settings, "Reset settings"))
		s.ResetSettings();

	// Apply shader
	//if (shader_on)
	//	BeginShaderMode(shader);

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