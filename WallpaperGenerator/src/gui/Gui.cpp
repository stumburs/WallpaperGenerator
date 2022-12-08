#include "Gui.h"
#include "raygui.h"

#include <iostream>
Gui::Gui(int kWindowWidth, int kWindowHeight, Generator *generator)
{
	this->generator = generator;

	this->kWindowWidth = kWindowWidth;
	this->kWindowHeight = kWindowHeight;

	// Main menu
	create_rect = { (float)kWindowWidth / 2 - 450, (float)kWindowHeight / 2 - 50, 400, 100 };
	view_rect = { (float)kWindowWidth / 2 + 50, (float)kWindowHeight / 2 - 50, 400, 100 };
	github_link_rect = { (float)kWindowWidth - 220, (float)kWindowHeight - 50 , 100, 40 };

	// Create menu
	flowfield_rect = { (float)kWindowWidth / 2 - 450, (float)kWindowHeight / 2 - 50, 400, 100 };
	shapes_rect = { (float)kWindowWidth / 2 + 50, (float)kWindowHeight / 2 - 50, 400, 100 };

	// Generate
	preview_rect = { (float)kWindowWidth - 850, 50, 800, 450 };
	shader_checkbox = { (float)kWindowWidth - 850, 525, 40, 40 };
	blend_slider = { (float)kWindowWidth - 650, 525, 400, 40 };
	save_image = { (float)kWindowWidth - 850, 585, 40, 40 };
	reset_image = { (float)kWindowWidth - 650, 585, 40, 40 };
	update_settings = { (float)kWindowWidth - 850, 645, 40, 40 };
	reset_settings = { (float)kWindowWidth - 650, 645, 40, 40 };

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
	case Gui::Menu::GENERATOR:

		switch (generator->active_generator)
		{
		case Generator::Generators::NONE:
			break;
		case Generator::Generators::FLOWFIELD:
			FlowfieldScreen();
			break;
		case Generator::Generators::SHAPES:
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
	if (generator->active_generator != Generator::NONE)
		this->preview_texture = generator->GetImage();
}

void Gui::MainMenuScreen()
{
	if (GuiButton(create_rect, "Create"))
	{
		active_menu = Menu::CREATE;
	}
	GuiButton(view_rect, "View");

	DrawText(version_number.c_str(), kWindowWidth - MeasureText(version_number.c_str(), 20) - 20, kWindowHeight - 40, 20, BLACK);

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
		generator->active_generator = Generator::FLOWFIELD;
		SetTargetFPS(0);
	}
	if (GuiButton(shapes_rect, "Shapes"))
	{
		active_menu = Menu::GENERATOR;
		generator->active_generator = Generator::SHAPES;
	}
}
void Gui::FlowfieldScreen()
{
	//shader_on = GuiCheckBox(shader_checkbox, "SHADER", shader_on);

	Vector2 mouse_pos = GetMousePosition();

	generator->SetValue("active_blend_mode", GuiSliderBar(blend_slider, NULL, NULL, generator->GetValue("active_blend_mode"), 0, 6));

	Rectangle scissor_area = GuiScrollPanel({ 50, 50, 650, 800 }, NULL, { 50, 50, 635, 1000 }, &scroll_pos);
	BeginScissorMode(scissor_area.x, scissor_area.y, scissor_area.width, scissor_area.height);

	// Apply scrollpos
	std::copy(std::begin(flowfield_setting_rects), std::end(flowfield_setting_rects), std::begin(flowfield_setting_rects_and_scroll_pos));
	for (Rectangle& rect : flowfield_setting_rects_and_scroll_pos)
		rect.y += scroll_pos.y;

	// Render and get values from sliders
	int index = 0;
	for (const auto& key : flowfield_settings_keys)
	{
		float value = GuiSliderBar(
			flowfield_setting_rects_and_scroll_pos[index],
			TextFormat("%0.*f", flowfield_settings_precision[key], generator->GetValue(key)),
			key.c_str(),
			generator->GetValue(key),
			flowfield_settings_ranges.at(key).first,
			flowfield_settings_ranges.at(key).second
		);

		generator->SetValue(key, value);
		index++;
	}

	// Display tooltips
	for (int i = 0; i < flowfield_tooltips.size(); i++)
	{
		Rectangle rect = { flowfield_setting_rects[i].x, flowfield_setting_rects[i].y + scroll_pos.y, flowfield_setting_rects[i].width, flowfield_setting_rects[i].height };
		if (CheckCollisionPointRec(mouse_pos, rect))
			DrawText(flowfield_tooltips[i].c_str(), mouse_pos.x, mouse_pos.y - 30, 20, DARKGRAY);
	}

	EndScissorMode();

	if (GuiLabelButton(save_image, "Save Image"))
	{
		Image img = LoadImageFromTexture(generator->GetImage());
		ImageFlipVertical(&img);
		ExportImage(img, "image.png");
	}

	if (GuiLabelButton(update_settings, "Update settings"))
		generator->UpdateSettings();

	if (GuiLabelButton(reset_settings, "Reset settings"))
		generator->ResetToDefault();

	// Apply shader
	//if (shader_on)
	//	BeginShaderMode(shader);

	DrawTexturePro(preview_texture, { 0, 0, (float)preview_texture.width, (float)-preview_texture.height }, preview_rect, { 0, 0 }, 0.0f, WHITE);
	//EndShaderMode();

	DrawText("Preview", kWindowWidth - 425 - MeasureText("PREVIEW", 40) / 2, 250, 40, { 255, 255, 255, 60 });

	// Blend slider text
	std::string blend_mode_name = blend_mode_names[(int)generator->GetValue("active_blend_mode")];
	DrawText(blend_mode_name.c_str(), blend_slider.x + blend_slider.width / 2 - (float)MeasureText(blend_mode_name.c_str(), 20) / 2, blend_slider.y + 10, 20, BLACK);

}

void Gui::ShapesScreen()
{
	// Preview Box
	DrawRectangleRec(preview_rect, WHITE);

	//shader_on = GuiCheckBox({ kWindowWidth - 850, 525, 40, 40 }, "SHADER", shader_on);

	//active_blend_mode = GuiSliderBar(blend_slider, NULL, NULL, active_blend_mode, 0, 6);
	generator->SetValue("active_blend_mode", GuiSliderBar(blend_slider, NULL, NULL, generator->GetValue("active_blend_mode"), 0, 6));

	switch ((int)generator->GetValue("active_blend_mode"))
	{
	case 0:
		DrawText("BLEND_ALPHA", blend_slider.x + blend_slider.width / 2 - (float)MeasureText("BLEND_ALPHA", 20) / 2, blend_slider.y + 10, 20, BLACK);
		break;
	case 1:
		DrawText("BLEND_ADDITIVE", blend_slider.x + blend_slider.width / 2 - (float)MeasureText("BLEND_ADDITIVE", 20) / 2, blend_slider.y + 10, 20, BLACK);
		break;
	case 2:
		DrawText("BLEND_MULTIPLIED", blend_slider.x + blend_slider.width / 2 - (float)MeasureText("BLEND_MULTIPLIED", 20) / 2, blend_slider.y + 10, 20, BLACK);
		break;
	case 3:
		DrawText("BLEND_ADD_COLORS", blend_slider.x + blend_slider.width / 2 - (float)MeasureText("BLEND_ADD_COLORS", 20) / 2, blend_slider.y + 10, 20, BLACK);
		break;
	case 4:
		DrawText("BLEND_SUBTRACT_COLORS", blend_slider.x + blend_slider.width / 2 - (float)MeasureText("BLEND_SUBTRACT_COLORS", 20) / 2, blend_slider.y + 10, 20, BLACK);
		break;
	case 5:
		DrawText("BLEND_ALPHA_PREMULTIPLY", blend_slider.x + blend_slider.width / 2 - (float)MeasureText("BLEND_ALPHA_PREMULTIPLY", 20) / 2, blend_slider.y + 10, 20, BLACK);
		break;
	case 6:
		DrawText("BLEND_CUSTOM", blend_slider.x + blend_slider.width / 2 - (float)MeasureText("BLEND_CUSTOM", 20) / 2, blend_slider.y + 10, 20, BLACK);
		break;
	default:
		break;
	}

	if (GuiLabelButton(save_image, "Save Image"))
	{
		Image img = LoadImageFromTexture(generator->GetImage());
		ImageFlipVertical(&img);
		ExportImage(img, "image.png");
	}

	if (GuiLabelButton(update_settings, "Update settings"))
	{
		generator->UpdateSettings();
	}

	if (GuiLabelButton(reset_settings, "Reset settings"))
	{
		generator->ResetToDefault();
	}

	// Apply shader
	//if (shader_on)
	//	BeginShaderMode(shader);

	DrawTexturePro(preview_texture, { 0, 0, (float)preview_texture.width, (float)-preview_texture.height }, preview_rect, { 0, 0 }, 0.0f, WHITE);

	//DrawTexturePro(s.image.texture, { 0, 0, (float)s.image.texture.width, (float)-s.image.texture.height }, { kWindowWidth - 850, 50, 800, 450 }, { 0, 0 }, 0.0f, WHITE);
	EndShaderMode();

	DrawText("Preview", kWindowWidth - 425 - MeasureText("PREVIEW", 40) / 2, 250, 40, { 255, 255, 255, 60 });
}