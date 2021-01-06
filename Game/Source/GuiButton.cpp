#include "GuiButton.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "ModuleFonts.h"


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

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
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
	case GuiControlState::DISABLED: app->render->DrawRectangle(bounds, 100, 100, 100, 100);
		break;
	case GuiControlState::NORMAL:
		app->render->DrawTexture(texture, bounds.x, bounds.y, &normalBtn);
		if (id == 1) { app->fonts->BlitText(bounds.x + 15, bounds.y + 5, font, text); }
        break;
    case GuiControlState::FOCUSED: 
		app->render->DrawTexture(texture, bounds.x, bounds.y, &focusedBtn);
        break;
    case GuiControlState::PRESSED: app->render->DrawRectangle(bounds, 0, 100, 255, 100);
		app->render->DrawTexture(texture, bounds.x, bounds.y, &pressedBtn);
        break;
    default:
        break;
    }

    return false;
}
