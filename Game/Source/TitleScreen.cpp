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
#include "ModuleFonts.h"

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
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->scene->cameraPos = { 0,0 };
	app->audio->PlayMusic("Assets/Audio/Music/game_over.ogg");
	app->transition->TransitionStep(nullptr, this, true, 30.0f);
	backgroundTexture = app->tex->Load("Assets/TitleScreen/title_screen.png");
	gameTitle= app->tex->Load("Assets/TitleScreen/game_title.png");
	menuBackgroundTexture = app->tex->Load("Assets/TitleScreen/menu_background.png");
	font = app->fonts->Load("Assets/font2.png", "0123456789:?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ", 1);
	font2 = app->fonts->Load("Assets/font2.1.png", "0123456789:?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ", 1);

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

	if (!app->CheckSaveFile())
	{
		btnContinue->state = GuiControlState::DISABLED;
	}

	settingsOn = false;
	creditsOn = false;
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


bool TitleScreen::PostUpdate()
{
	if (exit) { return false; }

	app->render->DrawTexture(backgroundTexture, 0, 0, false);
	app->render->DrawTexture(gameTitle, 0, 0, false);

	btnStart->Draw();
	btnContinue->Draw();
	btnSettings->Draw();
	btnCredits->Draw();
	btnQuit->Draw();

	if (settingsOn || creditsOn) { app->render->DrawTexture(menuBackgroundTexture, 90, 140, false); }

	sldMusicVolume->Draw();
	sldFxVolume->Draw();
	cbFullscreen->Draw();
	cbVSync->Draw();

	if (creditsOn)
	{
		app->fonts->BlitText(200, 210, font, "Developers:");
		app->fonts->BlitText(200, 250, font2, "Arnau Lucena: Weedzard");
		app->fonts->BlitText(200, 275, font2, "Abraham Diaz: Theran1");
		app->fonts->BlitText(200, 310, font, "Licenses:");
		app->fonts->BlitText(200, 350, font2, "This project is licensed under an unmodified MIT");
		app->fonts->BlitText(200, 370, font2, "license, which is an OSI-certified license that");
		app->fonts->BlitText(200, 390, font2, "allows static linking with closed source software.");
		app->fonts->BlitText(200, 430, font2, "Check LICENSE for further details.");
	}

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
		creditsOn = false;
		sldMusicVolume->state = GuiControlState::DISABLED;
		sldFxVolume->state = GuiControlState::DISABLED;
		cbFullscreen->state = GuiControlState::DISABLED;
		cbVSync->state = GuiControlState::DISABLED;
		app->transition->TransitionStep(this, (Module*)app->scene, false, 30.0f);
	} break;
	case 2://Continue
	{
		settingsOn = false;
		creditsOn = false;
		sldMusicVolume->state = GuiControlState::DISABLED;
		sldFxVolume->state = GuiControlState::DISABLED;
		cbFullscreen->state = GuiControlState::DISABLED;
		cbVSync->state = GuiControlState::DISABLED;
		continueOn = true;
		app->transition->TransitionStep(this, (Module*)app->scene, false, 30.0f);
	} break;
	case 3://Settings
	{
		creditsOn = false;
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
	} break;
	case 4: //Credits
	{
		settingsOn = false;
		sldMusicVolume->state = GuiControlState::DISABLED;
		sldFxVolume->state = GuiControlState::DISABLED;
		cbFullscreen->state = GuiControlState::DISABLED;
		cbVSync->state = GuiControlState::DISABLED;
		if (!creditsOn) { creditsOn = true; }
		else { creditsOn = false; }
	} break;
	case 5: //Quit
	{
		exit = true;
	} break;
	case 6: //Music volume
	{

	} break;
	case 7: //Fx Volume
	{
		if (!app->vsync) { app->vsync = true; }
		else { app->vsync = false; }
	} break;
	case 8: //Toggle Fullscreen
	{
		app->win->ToggleFullscreen(cbFullscreen->checked);
	} break;
	case 9: //Toggle VSync
	{

	} break;
	default:
		break;
	}
	return true;
}

void TitleScreen::Init() { active = false; }