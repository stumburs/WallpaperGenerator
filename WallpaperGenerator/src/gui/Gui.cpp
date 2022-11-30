#include "Gui.h"
#include "raygui.h"

Gui::Gui(int kWindowWidth, int kWindowHeight, Generator *generator)
{
	this->generator = generator;

	this->kWindowWidth = kWindowWidth;
	this->kWindowHeight = kWindowHeight;

	// Main menu
	create_rect = { (float)kWindowWidth / 2 - 450, (float)kWindowHeight / 2 - 50, 400, 100 };
	view_rect = { (float)kWindowWidth / 2 + 50, (float)kWindowHeight / 2 - 50, 400, 100 };

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

	window_width_rect = { 60, 40, 400, 40 };
	window_height_rect = { 60, 100, 400, 40 };
	scale_rect = { 60, 160, 400, 40 };
	seed_rect = { 60, 220, 400, 40 };
	flowfield_strength_rect = { 60, 280, 400, 40 };
	particle_count_rect = { 60, 340, 400, 40 };
	particle_speed_rect = { 60, 400, 400, 40 };
	particle_size_rect = { 60, 460, 400, 40 };
	particle_strength_rect = { 60, 520, 400, 40 };
	noise_height_rect = { 60, 580, 400, 40 };
	noise_detail_rect = { 60, 640, 400, 40 };
	x_mult_rect = { 60, 700, 400, 40 };
	y_mult_rect = { 60, 760, 400, 40 };
	z_mult_rect = { 60, 820, 400, 40 };
	z_rect = { 60, 880, 400, 40 };

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

		switch (active_algorithm)
		{
		case Gui::Algorithm::NONE:
			break;
		case Gui::Algorithm::FLOWFIELD:
			FlowfieldScreen();
			break;
		case Gui::Algorithm::SHAPES:
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
}

void Gui::CreateScreen()
{
	if (GuiButton(flowfield_rect, "Flowfield"))
	{
		active_menu = Menu::GENERATOR;
		active_algorithm = Algorithm::FLOWFIELD;
		generator->active_generator = Generator::FLOWFIELD;
	}
	if (GuiButton(shapes_rect, "Shapes"))
	{
		active_menu = Menu::GENERATOR;
		active_algorithm = Algorithm::SHAPES;
	}
}
void Gui::FlowfieldScreen()
{
	// Preview Box
	DrawRectangleRec(preview_rect, WHITE);

	//shader_on = GuiCheckBox(shader_checkbox, "SHADER", shader_on);

	generator->SetValue("active_blend_mode", GuiSliderBar(blend_slider, NULL, NULL, generator->GetValue("active_blend_mode"), 0, 6));


	/*
	*  !!!!!!!!!!!!!!!
	*
	*	REFACTOR ASAP
	*
	*  !!!!!!!!!!!!!!!
	*/

	Rectangle scissor_area = GuiScrollPanel({ 50, 50, 650, 800 }, NULL, { 50, 50, 635, 1000 }, &scroll_pos);
	BeginScissorMode(scissor_area.x, scissor_area.y, scissor_area.width, scissor_area.height);

	// User values
	generator->SetValue("window_width", (int)GuiSliderBar({ window_width_rect.x + 50, window_width_rect.y + 50 + scroll_pos.y, window_width_rect.width, window_width_rect.height }, TextFormat("%d", (int)generator->GetValue("window_width")), "Image Width", (int)generator->GetValue("window_width"), 1, 1920));
	generator->SetValue("window_height", (int)GuiSliderBar({ window_height_rect.x + 50, window_height_rect.y + 50 + scroll_pos.y, window_height_rect.width, window_height_rect.height }, TextFormat("%d", (int)generator->GetValue("window_height")), "Image Height", (int)generator->GetValue("window_height"), 1, 1080));
	generator->SetValue("scale", (int)GuiSliderBar({ scale_rect.x + 50, scale_rect.y + 50 + scroll_pos.y, scale_rect.width, scale_rect.height }, TextFormat("%d", (int)generator->GetValue("scale")), "Image Scale", (int)generator->GetValue("scale"), 1, 40));
	//GuiDrawText("(seed input)", seed_rect, GuiTextAlignment::TEXT_ALIGN_CENTER, BLACK);
	generator->SetValue("flowfield_strength", GuiSliderBar({ flowfield_strength_rect.x + 50, flowfield_strength_rect.y + 50 + scroll_pos.y, flowfield_strength_rect.width, flowfield_strength_rect.height }, TextFormat("%0.3f", generator->GetValue("flowfield_strength")), "Flowfield Strength", generator->GetValue("flowfield_strength"), 0.001f, 0.1f));
	generator->SetValue("particle_count", (int)GuiSliderBar({ particle_count_rect.x + 50, particle_count_rect.y + 50 + scroll_pos.y, particle_count_rect.width, particle_count_rect.height }, TextFormat("%d", (int)generator->GetValue("particle_count")), "Particle count", (int)generator->GetValue("particle_count"), 500, 50000));
	generator->SetValue("particle_speed", GuiSliderBar({ particle_speed_rect.x + 50, particle_speed_rect.y + 50 + scroll_pos.y, particle_speed_rect.width, particle_speed_rect.height }, TextFormat("%0.1f", generator->GetValue("particle_speed")), "Particle Speed", generator->GetValue("particle_speed"), 0.0f, 2.0f));
	//GuiDrawText("(particle size input)", particle_size_rect, GuiTextAlignment::TEXT_ALIGN_CENTER, BLACK);
	generator->SetValue("particle_strength", (unsigned char)GuiSliderBar({ particle_strength_rect.x + 50, particle_strength_rect.y + 50 + scroll_pos.y, particle_strength_rect.width, particle_strength_rect.height }, TextFormat("%d", (unsigned char)generator->GetValue("particle_strength")), "Particle Strength", (unsigned char)generator->GetValue("particle_strength"), 1, 16));
	//GuiDrawText("(noise height input)", noise_height_rect, GuiTextAlignment::TEXT_ALIGN_CENTER, BLACK);
	generator->SetValue("noise_detail", (int)GuiSliderBar({ noise_detail_rect.x + 50, noise_detail_rect.y + 50 + scroll_pos.y, noise_detail_rect.width, noise_detail_rect.height }, TextFormat("%d", (int)generator->GetValue("noise_detail")), "Noise Detail", (int)generator->GetValue("noise_detail"), 1, 16));
	generator->SetValue("x_mult", GuiSliderBar({ x_mult_rect.x + 50, x_mult_rect.y + 50 + scroll_pos.y, x_mult_rect.width, x_mult_rect.height }, TextFormat("%0.2f", generator->GetValue("x_mult")), "X-Axis Multiplier", generator->GetValue("x_mult"), 0.01f, 0.2f));
	generator->SetValue("y_mult", GuiSliderBar({ y_mult_rect.x + 50, y_mult_rect.y + 50 + scroll_pos.y, y_mult_rect.width, y_mult_rect.height }, TextFormat("%0.2f", generator->GetValue("y_mult")), "Y-Axis Multiplier", generator->GetValue("y_mult"), 0.01f, 0.2f));
	generator->SetValue("z_mult", GuiSliderBar({ z_mult_rect.x + 50, z_mult_rect.y + 50 + scroll_pos.y, z_mult_rect.width, z_mult_rect.height }, TextFormat("%0.2f", generator->GetValue("z_mult")), "Z-Axis Multiplier", generator->GetValue("z_mult"), 0.01f, 0.2f));
	if (CheckCollisionPointRec(GetMousePosition(), { window_width_rect.x + 50, window_width_rect.y + 50 + scroll_pos.y, window_width_rect.width, window_width_rect.height }))
		DrawText("Final image horizontal resolution", GetMousePosition().x, GetMousePosition().y - 30, 20, DARKGRAY);
	if (CheckCollisionPointRec(GetMousePosition(), { window_height_rect.x + 50, window_height_rect.y + 50 + scroll_pos.y, window_height_rect.width, window_height_rect.height }))
		DrawText("Final image horizontal resolution", GetMousePosition().x, GetMousePosition().y - 30, 20, DARKGRAY);
	if (CheckCollisionPointRec(GetMousePosition(), { scale_rect.x + 50, scale_rect.y + 50 + scroll_pos.y, scale_rect.width, scale_rect.height }))
		DrawText("Lower number = more detail, but worse performance", GetMousePosition().x, GetMousePosition().y - 30, 20, DARKGRAY);
	if (CheckCollisionPointRec(GetMousePosition(), { flowfield_strength_rect.x + 50, flowfield_strength_rect.y + 50 + scroll_pos.y, flowfield_strength_rect.width, flowfield_strength_rect.height }))
		DrawText("How accurately particles follow the flowfield", GetMousePosition().x, GetMousePosition().y - 30, 20, DARKGRAY);
	if (CheckCollisionPointRec(GetMousePosition(), { particle_count_rect.x + 50, particle_count_rect.y + 50 + scroll_pos.y, particle_count_rect.width, particle_count_rect.height }))
		DrawText("Amount of particles", GetMousePosition().x, GetMousePosition().y - 30, 20, DARKGRAY);
	if (CheckCollisionPointRec(GetMousePosition(), { particle_speed_rect.x + 50, particle_speed_rect.y + 50 + scroll_pos.y, particle_speed_rect.width, particle_speed_rect.height }))
		DrawText("How quickly particles move", GetMousePosition().x, GetMousePosition().y - 30, 20, DARKGRAY);
	if (CheckCollisionPointRec(GetMousePosition(), { particle_strength_rect.x + 50, particle_strength_rect.y + 50 + scroll_pos.y, particle_strength_rect.width, particle_strength_rect.height }))
		DrawText("Particle color intensity", GetMousePosition().x, GetMousePosition().y - 30, 20, DARKGRAY);
	if (CheckCollisionPointRec(GetMousePosition(), { noise_detail_rect.x + 50, noise_detail_rect.y + 50 + scroll_pos.y, noise_detail_rect.width, noise_detail_rect.height }))
		DrawText("Perlin noise octaves. Higher number = worse performance", GetMousePosition().x, GetMousePosition().y - 30, 20, DARKGRAY);
	if (CheckCollisionPointRec(GetMousePosition(), { x_mult_rect.x + 50, x_mult_rect.y + 50 + scroll_pos.y, x_mult_rect.width, x_mult_rect.height }))
		DrawText("Noise x-axis multiplier", GetMousePosition().x, GetMousePosition().y - 30, 20, DARKGRAY);
	if (CheckCollisionPointRec(GetMousePosition(), { y_mult_rect.x + 50, y_mult_rect.y + 50 + scroll_pos.y, y_mult_rect.width, y_mult_rect.height }))
		DrawText("Noise y-axis multiplier", GetMousePosition().x, GetMousePosition().y - 30, 20, DARKGRAY);
	if (CheckCollisionPointRec(GetMousePosition(), { z_mult_rect.x + 50, z_mult_rect.y + 50 + scroll_pos.y, z_mult_rect.width, z_mult_rect.height }))
		DrawText("Noise z-axis multiplier", GetMousePosition().x, GetMousePosition().y - 30, 20, DARKGRAY);

	EndScissorMode();

	if (GuiLabelButton(save_image, "Save Image")) {
		Image img = LoadImageFromTexture(generator->GetImage());
		ImageFlipVertical(&img);
		ExportImage(img, "image.png");
	}

	if (GuiLabelButton(update_settings, "Update settings")) {
		generator->UpdateSettings();
	}

	if (GuiLabelButton(reset_settings, "Reset settings")) {
		generator->ResetToDefault();
	}

	// Apply shader
	//if (shader_on)
	//	BeginShaderMode(shader);

	DrawTexturePro(preview_texture, { 0, 0, (float)preview_texture.width, (float)-preview_texture.height }, preview_rect, { 0, 0 }, 0.0f, WHITE);
	EndShaderMode();

	DrawText("Preview", kWindowWidth - 425 - MeasureText("PREVIEW", 40) / 2, 250, 40, { 255, 255, 255, 60 });

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
}

void Gui::ShapesScreen()
{
	// Preview Box
	DrawRectangle(kWindowWidth - 850, 50, 800, 450, BLACK);

	//shader_on = GuiCheckBox({ kWindowWidth - 850, 525, 40, 40 }, "SHADER", shader_on);

	//active_blend_mode = GuiSliderBar(blend_slider, NULL, NULL, active_blend_mode, 0, 6);

	/*switch (active_blend_mode)
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
	}*/

	// Apply shader
	//if (shader_on)
	//	BeginShaderMode(shader);

	//DrawTexturePro(s.image.texture, { 0, 0, (float)s.image.texture.width, (float)-s.image.texture.height }, { kWindowWidth - 850, 50, 800, 450 }, { 0, 0 }, 0.0f, WHITE);
	EndShaderMode();

	DrawText("Preview", kWindowWidth - 425 - MeasureText("PREVIEW", 40) / 2, 250, 40, { 255, 255, 255, 60 });
}