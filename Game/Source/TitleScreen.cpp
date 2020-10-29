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

#include "SDL/include/SDL_scancode.h"

TitleScreen::TitleScreen() : Module() {
	name.Create("titleScreen");
	default.PushBack({ 0,0,1280,160 });
	default.PushBack({ 0,0,1,1 });
	default.SetSpeed(0.0003f);
}

TitleScreen::~TitleScreen() {}

// Load assets
bool TitleScreen::Start() {
	app->transition->TransitionStep(nullptr, this, true, 1200.0f);
	backgroundTexture = app->tex->Load("Assets/Title Screen/TitleScreenBackground.png");
	gameTitle= app->tex->Load("Assets/Title Screen/GameTitle.png");
	pressToStartTexture = app->tex->Load("Assets/Title Screen/PressEnterToStart.png");
	app->player->isAlive = true;
	return true;
}

bool TitleScreen::PreUpdate()
{
	
	return true;
}

bool TitleScreen::Update(float dt) {
	pressToStart = &default;
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		app->transition->TransitionStep(this, (Module*)app->scene, false, 200.0f);
	}
	return true;
}

// Update: draw background
bool TitleScreen::PostUpdate() {
	// Draw everything --------------------------------------
	app->render->DrawTexture(backgroundTexture, 0, 0, false);
	app->render->DrawTexture(gameTitle, 157, 120, false);

	pressToStart->Update();
	app->render->DrawTexture(pressToStartTexture, 0, 450, &pressToStart->GetCurrentFrame(), false);
	return true;
}

bool TitleScreen::CleanUp() {
	app->tex->UnLoad(backgroundTexture);
	app->tex->UnLoad(gameTitle);
	return true;
}


void TitleScreen::Init()
{
	active = false;
}