#include "GuiManager.h"
#include "App.h"
#include "Textures.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"

GuiManager::GuiManager() :Module()
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	texture = app->tex->Load("Assets/darkSheet.png");
	//Put font

	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds)
{
	GuiControl* control = nullptr;


	// Create the corresponding GuiControl type
	//case GuiControlType::BUTTON: control = new GuiButton(1234);  break;

	//btnStart = new GuiButton(1, { 1280 / 2 - 300 / 2, 400, 300, 80 }, "START");
	//btnStart->SetObserver(this);

	switch (type)
	{
	case GuiControlType::BUTTON:
		control = new GuiButton(id, bounds, text);
		break;
	case GuiControlType::CHECKBOX:
		control = new GuiCheckBox(id, bounds, text);
		break;
	case GuiControlType::SLIDER:
		control = new GuiSlider(id, bounds, sliderBounds, text);
		break;
	default:
		break;
	}

	control->SetObserver(observer);
	control->SetTexture(texture);

	// Created entities are added to the list
	if (control != nullptr) controls.Add(control);

	return control;
}

bool GuiManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	//UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}

