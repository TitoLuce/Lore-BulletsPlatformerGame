#include "Logo Screen.h"

#include "App.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Transition.h"
#include "Scene.h"
#include "Input.h"

#include "SDL/include/SDL_scancode.h"

LogoScreen::LogoScreen() : Module() { name.Create("logoScreen"); }

LogoScreen::~LogoScreen() {}

// Load assets
bool LogoScreen::Start()
{
	app->transition->TransitionStep(nullptr, this, true, 30.0f);
	logoTitleTexture = app->tex->Load("Assets/logo_alpha.png");
	return true;
}

bool LogoScreen::PreUpdate() { return true; }

bool LogoScreen::Update(float dt)
{
	if (timer < 120.0f)
	{
		timer++;
	}
	else
	{
		app->transition->TransitionStep(this, (Module*)app->titleScreen, false, 30.0f);
	}
	return true;
}

// Update: draw background
bool LogoScreen::PostUpdate()
{
	// Draw everything --------------------------------------
	app->render->DrawTexture(logoTitleTexture, 0, 0, false);
	return true;
}

bool LogoScreen::CleanUp()
{
	app->tex->UnLoad(logoTitleTexture);
	return true;
}

void LogoScreen::Init() { active = true; }