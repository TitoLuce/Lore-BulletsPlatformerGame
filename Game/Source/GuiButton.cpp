#include "GuiButton.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "ModuleFonts.h"
#include "Audio.h"
#include "Scene.h"


GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	normalBtn = { 0,286,189,44 };
	focusedBtn = { 0,98,189,44 };
	pressedBtn = { 0,144,189,44 };
}

GuiButton::~GuiButton()
{
}

bool GuiButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);

		if ((mouseX > bounds.x /*- app->scene->cameraPos.x*/) && (mouseX < (bounds.x + bounds.w /*- app->scene->cameraPos.x*/)) &&
			(mouseY > bounds.y /*- app->scene->cameraPos.y*/) && (mouseY < (bounds.y + bounds.h /*- app->scene->cameraPos.y*/)))
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
		else state = GuiControlState::NORMAL;
	}

	return false;
}

bool GuiButton::Draw()
{
	// Draw the right button depending on state
	switch (state)
	{
	case GuiControlState::DISABLED: //app->render->DrawRectangle(bounds, 100, 100, 100, 100);
		app->render->DrawTexture(texture, bounds.x, bounds.y, &focusedBtn);
		if (id == 1) { app->fonts->BlitText(bounds.x + 15, bounds.y + 5, font4, text); }
		if (id == 2) { app->fonts->BlitText(bounds.x + 25, bounds.y + 15, font41, text); }
		if (id == 3) { app->fonts->BlitText(bounds.x + 30, bounds.y + 15, font41, text); }
		if (id == 4) { app->fonts->BlitText(bounds.x + 35, bounds.y + 15, font41, text); }
		if (id == 5) { app->fonts->BlitText(bounds.x + 30, bounds.y + 5, font4, text); }
		if (id == 10) { app->fonts->BlitText(bounds.x + 45, bounds.y + 15, font41, text); }
		if (id == 11) { app->fonts->BlitText(bounds.x + 3, bounds.y + 15, font41, text); }
		if (id == 12) { app->fonts->BlitText(bounds.x + 60, bounds.y + 15, font41, text); }

		break;
	case GuiControlState::NORMAL:
		clickPlay = true;
		hoverPlay = true;
		app->render->DrawTexture(texture, bounds.x, bounds.y, &normalBtn);
		if (id == 1) { app->fonts->BlitText(bounds.x + 15, bounds.y + 5, font, text); }
		if (id == 2) { app->fonts->BlitText(bounds.x + 25, bounds.y + 15, font11, text); }
		if (id == 3) { app->fonts->BlitText(bounds.x + 30, bounds.y + 15, font11, text); }
		if (id == 4) { app->fonts->BlitText(bounds.x + 35, bounds.y + 15, font11, text); }
		if (id == 5) { app->fonts->BlitText(bounds.x + 30, bounds.y + 5, font, text); }
		if (id == 10) { app->fonts->BlitText(bounds.x + 45, bounds.y + 15, font11, text); }
		if (id == 11) { app->fonts->BlitText(bounds.x + 3, bounds.y + 15, font11, text); }
		if (id == 12) { app->fonts->BlitText(bounds.x + 60, bounds.y + 15, font11, text); }
		break;
	case GuiControlState::FOCUSED: 
		clickPlay = true;
		if (hoverPlay)
		{
			app->audio->PlayFx(hover);
			hoverPlay = false;
		}
		app->render->DrawTexture(texture, bounds.x, bounds.y, &focusedBtn);
		if (id == 1) { app->fonts->BlitText(bounds.x + 15, bounds.y + 5, font2, text); }
		if (id == 2) { app->fonts->BlitText(bounds.x + 25, bounds.y + 15, font21, text); }
		if (id == 3) { app->fonts->BlitText(bounds.x + 30, bounds.y + 15, font21, text); }
		if (id == 4) { app->fonts->BlitText(bounds.x + 35, bounds.y + 15, font21, text); }
		if (id == 5) { app->fonts->BlitText(bounds.x + 30, bounds.y + 5, font2, text); }
		if (id == 10) { app->fonts->BlitText(bounds.x + 45, bounds.y + 15, font21, text); }
		if (id == 11) { app->fonts->BlitText(bounds.x + 3, bounds.y + 15, font21, text); }
		if (id == 12) { app->fonts->BlitText(bounds.x + 60, bounds.y + 15, font21, text); }
		break;
	case GuiControlState::PRESSED: //app->render->DrawRectangle(bounds, 0, 100, 255, 100);
		if (clickPlay)
		{
			app->audio->PlayFx(click);
			clickPlay = false;
		}
		app->render->DrawTexture(texture, bounds.x, bounds.y, &pressedBtn);
		if (id == 1) { app->fonts->BlitText(bounds.x + 15, bounds.y + 5, font3, text); }
		if (id == 2) { app->fonts->BlitText(bounds.x + 25, bounds.y + 15, font31, text); }
		if (id == 3) { app->fonts->BlitText(bounds.x + 30, bounds.y + 15, font31, text); }
		if (id == 4) { app->fonts->BlitText(bounds.x + 35, bounds.y + 15, font31, text); }
		if (id == 5) { app->fonts->BlitText(bounds.x + 30, bounds.y + 5, font3, text); }
		if (id == 10) { app->fonts->BlitText(bounds.x + 45, bounds.y + 15, font31, text); }
		if (id == 11) { app->fonts->BlitText(bounds.x + 3, bounds.y + 15, font31, text); }
		if (id == 12) { app->fonts->BlitText(bounds.x + 60, bounds.y + 15, font31, text); }
		break;
	default:
		break;
	}

	return false;
}
