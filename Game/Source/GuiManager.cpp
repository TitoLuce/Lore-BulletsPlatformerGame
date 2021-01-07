#include "GuiManager.h"
#include "App.h"
#include "Textures.h"
#include "ModuleFonts.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "Audio.h"

GuiManager::GuiManager() :Module()
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	//Texture fonts & fx
	texture = app->tex->Load("Assets/darkSheet.png");

	font = app->fonts->Load("Assets/font.png", "0123456789:?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ", 1);
	font2 = app->fonts->Load("Assets/font2.png", "0123456789:?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ", 1);
	font3 = app->fonts->Load("Assets/font3.png", "0123456789:?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ", 1);
	font4 = app->fonts->Load("Assets/font4.png", "0123456789:?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ", 1);
	font11 = app->fonts->Load("Assets/font1.1.png", "0123456789:?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ", 1);
	font21 = app->fonts->Load("Assets/font2.1.png", "0123456789:?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ", 1);
	font31 = app->fonts->Load("Assets/font3.1.png", "0123456789:?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ", 1);
	font41 = app->fonts->Load("Assets/font4.1.png", "0123456789:?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ", 1);

	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds)
{
	GuiControl* control = nullptr;

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
	control->SetFont(font, font2, font3, font4, font11, font21, font31, font41);

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

