#include "GuiSlider.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "ModuleFonts.h"
#include "Audio.h"

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

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        app->input->GetMousePosition(mouseX, mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
			}
        }
        else state = GuiControlState::NORMAL;
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
		app->render->DrawTexture(texture, sliderBounds.x, sliderBounds.y, &sliderButton);
		if (id == 6) { app->fonts->BlitText(bounds.x + 90, bounds.y - 30, font21, text); }
		if (id == 7) { app->fonts->BlitText(bounds.x + 120, bounds.y - 30, font21, text); }
		//app->render->DrawRectangle(sliderBounds, 0, 255, 0, 100);
        break;
    case GuiControlState::FOCUSED: //app->render->DrawRectangle(bounds, 255, 255, 0);
		clickPlay = true;
		if (hoverPlay)
		{
			app->audio->PlayFx(hover, 100);
			hoverPlay = false;
		}
		app->render->DrawTexture(texture, sliderBounds.x, sliderBounds.y, &slider);
		app->render->DrawTexture(texture, sliderBounds.x, sliderBounds.y, &sliderButtonSelected);
		if (id == 6) { app->fonts->BlitText(bounds.x + 90, bounds.y - 30, font21, text); }
		if (id == 7) { app->fonts->BlitText(bounds.x + 120, bounds.y - 30, font21, text); }
        break;
    case GuiControlState::PRESSED: //app->render->DrawRectangle(bounds, 0, 255, 255, 255);
		if (clickPlay)
		{
			app->audio->PlayFx(click, 100);
			clickPlay = false;
		}
		app->render->DrawTexture(texture, sliderBounds.x, sliderBounds.y, &slider);
		app->render->DrawTexture(texture, sliderBounds.x, sliderBounds.y, &sliderButtonPressed);
		if (id == 6) { app->fonts->BlitText(bounds.x + 90, bounds.y - 30, font21, text); }
		if (id == 7) { app->fonts->BlitText(bounds.x + 120, bounds.y - 30, font21, text); }
        break;
    default:
        break;
    }

    return false;
}
