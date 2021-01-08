#include "TitleScreen.h"

#include "App.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "Player.h"
#include "Transition.h"
#include "Scene.h"
#include "Animation.h"
#include "Input.h"

#include "EntityManager.h"
#include "GuiManager.h"

#include "SDL/include/SDL_scancode.h"

TitleScreen::TitleScreen() : Module()
{
	name.Create("titleScreen");
}

TitleScreen::~TitleScreen() {}

// Load assets
bool TitleScreen::Start()
{
	app->audio->PlayMusic("Assets/Audio/Music/game_over.ogg");
	app->transition->TransitionStep(nullptr, this, true, 30.0f);
	backgroundTexture = app->tex->Load("Assets/TitleScreen/title_screen.png");
	gameTitle= app->tex->Load("Assets/TitleScreen/game_title.png");
	menuBackgroundTexture = app->tex->Load("Assets/TitleScreen/menu_background.png");
	
	btnStart = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Start", { 150, 600, 189, 44 }, this);
	btnContinue = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Continue", { 350, 600, 189, 44 }, this);
	btnSettings = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Settings", { 550, 600, 189, 44 }, this);
	btnCredits = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Credits", { 750, 600, 189, 44 }, this);
	btnQuit=(GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Quit", { 950, 600, 189, 44 }, this);

	sldMusicVolume = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 6, "Music Volume:", { 220, 250, 40, 40 }, this, { 220, 250, 400, 40 });
	sldFxVolume = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 7, "Fx Volume:", { 660, 250, 40, 40 }, this, { 660, 250, 400, 40 });

	cbFullscreen = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 8, "Fullscreen", { 490, 400, 40, 40 }, this);
	cbVSync = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 9, "VSync", { 890, 400, 40, 40 }, this);
	
	sldMusicVolume->state = GuiControlState::DISABLED;
	sldFxVolume->state = GuiControlState::DISABLED;

	cbFullscreen->state = GuiControlState::DISABLED;
	cbVSync->state = GuiControlState::DISABLED;

	bool settingsOn = false;
	return true;
}

bool TitleScreen::PreUpdate() { return true; }

bool TitleScreen::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) { app->transition->TransitionStep(this, (Module*)app->scene, false, 30.0f); }
	btnStart->Update(dt);
	btnContinue->Update(dt);
	btnSettings->Update(dt);
	btnCredits->Update(dt);
	btnQuit->Update(dt);
	sldMusicVolume->Update(dt);
	sldFxVolume->Update(dt);
	cbFullscreen->Update(dt);
	cbVSync->Update(dt);
	return true;
}

// Update: draw background
bool TitleScreen::PostUpdate()
{
	// Draw everything --------------------------------------
	

	app->render->DrawTexture(backgroundTexture, 0, 0, false);
	app->render->DrawTexture(gameTitle, 0, 0, false);

	btnStart->Draw();
	btnContinue->Draw();
	btnSettings->Draw();
	btnCredits->Draw();
	btnQuit->Draw();

	if (settingsOn) { app->render->DrawTexture(menuBackgroundTexture, 90, 140, false); }

	sldMusicVolume->Draw();
	sldFxVolume->Draw();
	cbFullscreen->Draw();
	cbVSync->Draw();

	return true;
}

bool TitleScreen::CleanUp()
{
	app->tex->UnLoad(backgroundTexture);
	app->tex->UnLoad(gameTitle);
	return true;
}

bool TitleScreen::OnGuiMouseClickEvent(GuiControl* control)
{

	switch (control->id)
	{
	case 1://Start
	{
		settingsOn = false;
		sldMusicVolume->state = GuiControlState::DISABLED;
		sldFxVolume->state = GuiControlState::DISABLED;
		cbFullscreen->state = GuiControlState::DISABLED;
		cbVSync->state = GuiControlState::DISABLED;
		app->transition->TransitionStep(this, (Module*)app->scene, false, 30.0f);
	}
	case 2://Continue
	{
		settingsOn = false;
		sldMusicVolume->state = GuiControlState::DISABLED;
		sldFxVolume->state = GuiControlState::DISABLED;
		cbFullscreen->state = GuiControlState::DISABLED;
		cbVSync->state = GuiControlState::DISABLED;
	}
	case 3://Settings
	{
		if (!settingsOn)
		{
			settingsOn = true;
			sldMusicVolume->state = GuiControlState::NORMAL;
			sldFxVolume->state = GuiControlState::NORMAL;
			cbFullscreen->state = GuiControlState::NORMAL;
			cbVSync->state = GuiControlState::NORMAL;
		}
		else
		{
			settingsOn = false;
			sldMusicVolume->state = GuiControlState::DISABLED;
			sldFxVolume->state = GuiControlState::DISABLED;
			cbFullscreen->state = GuiControlState::DISABLED;
			cbVSync->state = GuiControlState::DISABLED;
		}
	}
	case 4: //Credits
	{

	}
	case 5: //Quit
	{

	}
	case 6: //Music volume
	{

	}
	case 7: //Fx Volume
	{

	}
	case 8: //Toggle Fullscreen
	{

	}

	}
	return true;
}

void TitleScreen::Init() { active = false; }