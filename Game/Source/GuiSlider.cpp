#include "GuiSlider.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "ModuleFonts.h"
#include "Audio.h"
#include "Scene.h"
#include "TitleScreen.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, SDL_Rect sliderBounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;
	this->sliderBounds = sliderBounds;

	sliderButton = { 299,41,40,40  };
	sliderButtonSelected = { 392,41,40,40 };
	sliderButtonPressed = { 346,41,40,40 };
	slider = { 299,0,400, 40 };
}

GuiSlider::~GuiSlider() {}

bool GuiSlider::Update(float dt)
{

	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);

		if ((mouseX > bounds.x - app->scene->cameraPos.x) && (mouseX < (bounds.x + bounds.w - app->scene->cameraPos.x)) &&
			(mouseY > bounds.y - app->scene->cameraPos.y) && (mouseY < (bounds.y + bounds.h - app->scene->cameraPos.y)))
		{
			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
			}
		}
		else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) != KeyState::KEY_REPEAT) state = GuiControlState::NORMAL;
		if (state == GuiControlState::PRESSED)
		{
			value = bounds.x - sliderBounds.x * (128.0f / (float)(sliderBounds.w - bounds.w)) - 142;
			if (bounds.x - sliderBounds.x == 0) { value = 0; }
			int x;
			int y;
			app->input->GetMouseMotion(x, y);
			if (x != 0)
			{
				bounds.x = mouseX - bounds.w / 2 + app->scene->cameraPos.x;
				if (bounds.x < sliderBounds.x) { bounds.x = sliderBounds.x; }
				if (bounds.x + bounds.w > sliderBounds.x + sliderBounds.w) { bounds.x = sliderBounds.x + sliderBounds.w - bounds.w; }
			}
			NotifyObserver();
		}
	}


	

	return false;
}

bool GuiSlider::Draw()
{
	// Draw the right button depending on state
	switch (state)
	{
	case GuiControlState::DISABLED:
		break;
	case GuiControlState::NORMAL:
		hoverPlay = true;
		clickPlay = true;
		app->render->DrawTexture(texture, sliderBounds.x, sliderBounds.y, &slider);
		app->render->DrawTexture(texture, bounds.x, bounds.y, &sliderButton);
		if (id == 6) { app->fonts->BlitText(sliderBounds.x + 90, sliderBounds.y - 30, font21, text); }
		if (id == 7) { app->fonts->BlitText(sliderBounds.x + 120, sliderBounds.y - 30, font21, text); }
		if (app->render->drawButtonsColliders) { app->render->DrawRectangle(sliderBounds, 0, 255, 255, 100); }
		if (app->render->drawButtonsColliders) { app->render->DrawRectangle(bounds, 0, 255, 0, 100); }
		break;
	case GuiControlState::FOCUSED:
		clickPlay = true;
		if (hoverPlay)
		{
			app->audio->PlayFx(hover);
			hoverPlay = false;
		}
		app->render->DrawTexture(texture, sliderBounds.x, sliderBounds.y, &slider);
		app->render->DrawTexture(texture, bounds.x, bounds.y, &sliderButtonSelected);
		if (id == 6) { app->fonts->BlitText(sliderBounds.x + 90, sliderBounds.y - 30, font21, text); }
		if (id == 7) { app->fonts->BlitText(sliderBounds.x + 120, sliderBounds.y - 30, font21, text); }
		if (app->render->drawButtonsColliders) { app->render->DrawRectangle(sliderBounds, 0, 255, 255, 100); }
		if (app->render->drawButtonsColliders) { app->render->DrawRectangle(bounds, 255, 255, 0, 100); }
		break;
	case GuiControlState::PRESSED:
		if (clickPlay)
		{
			app->audio->PlayFx(click);
			clickPlay = false;
		}
		app->render->DrawTexture(texture, sliderBounds.x, sliderBounds.y, &slider);
		app->render->DrawTexture(texture, bounds.x, bounds.y, &sliderButtonPressed);
		if (id == 6) { app->fonts->BlitText(sliderBounds.x + 90, sliderBounds.y - 30, font21, text); }
		if (id == 7) { app->fonts->BlitText(sliderBounds.x + 120, sliderBounds.y - 30, font21, text); }
		if (app->render->drawButtonsColliders) { app->render->DrawRectangle(sliderBounds, 0, 255, 255, 100); }
		if (app->render->drawButtonsColliders) { app->render->DrawRectangle(bounds, 0, 255, 255, 100); }
		break;
	default:
		break;
	}
	return false;
}
