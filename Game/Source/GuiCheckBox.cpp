#include "GuiCheckBox.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "ModuleFonts.h"
#include "Audio.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
    this->bounds = bounds;
    this->text = text;

	normalCb = { 148,433,36,36 };
	focusedCb = { 186,433,36,36 };
	pressedCb = { 148,470,36,36 };
	check = { 223,469,36,36 };
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(float dt)
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
				if (!checked)
				{
					checked = true;
				}
				else
				{
					checked = false;
				}

                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiCheckBox::Draw()
{
    // Draw the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED:
    {
    } break;
    case GuiControlState::NORMAL: 
    {
		app->render->DrawTexture(texture, bounds.x, bounds.y, &normalCb);
		if (checked) { app->render->DrawTexture(texture, bounds.x, bounds.y, &check); }
		if (id == 8) { app->fonts->BlitText(bounds.x - 180, bounds.y + 8, font21, text); }
		if (id == 9) { app->fonts->BlitText(bounds.x - 100, bounds.y + 8, font21, text); }
    } break;
    case GuiControlState::FOCUSED:
		app->render->DrawTexture(texture, bounds.x, bounds.y, &focusedCb);
		if (checked) { app->render->DrawTexture(texture, bounds.x, bounds.y, &check); }
		if (id == 8) { app->fonts->BlitText(bounds.x - 180, bounds.y + 8, font21, text); }
		if (id == 9) { app->fonts->BlitText(bounds.x - 100, bounds.y + 8, font21, text); }
        break;
    case GuiControlState::PRESSED:
		app->render->DrawTexture(texture, bounds.x, bounds.y, &pressedCb);
		if (checked) { app->render->DrawTexture(texture, bounds.x, bounds.y, &check); }
		if (id == 8) { app->fonts->BlitText(bounds.x - 180, bounds.y + 8, font21, text); }
		if (id == 9) { app->fonts->BlitText(bounds.x - 100, bounds.y + 8, font21, text); }
        break;
    default:
        break;
    }

    return false;
}
