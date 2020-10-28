#include "TitleScreen.h"

#include "App.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
//#include "Player.h" TBA
#include "Transition.h"
#include "Scene.h"
#include "Animation.h"
#include "Input.h"

#include "SDL/include/SDL_scancode.h"

TitleScreen::TitleScreen() : Module() {
	name.Create("titleScreen");

}

TitleScreen::~TitleScreen() {}

// Load assets
bool TitleScreen::Start() {
	app->transition->TransitionStep(nullptr, this, true, 1200.0f);
	backgroundTexture = app->tex->Load("Assets/Title Screen/TitleScreenBackground.png");
	gameTitle= app->tex->Load("Assets/Title Screen/GameTitle.png");
	return true;
}

bool TitleScreen::PreUpdate()
{
	
	return true;
}

bool TitleScreen::Update(float dt) {
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		app->transition->TransitionStep(this, (Module*)app->scene, false, 200.0f);
	}
	return true;
}

// Update: draw background
bool TitleScreen::PostUpdate() {
	// Draw everything --------------------------------------
	app->render->DrawTexture(backgroundTexture, 0, 0, false);
	app->render->DrawTexture(gameTitle, 200, 300, false);
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