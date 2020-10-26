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

#include "SDL/include/SDL_scancode.h"

TitleScreen::TitleScreen() : Module() {
	name.Create("titleScreen");
}

TitleScreen::~TitleScreen() {}

// Load assets
bool TitleScreen::Start() {
	backgroundTexture = app->tex->Load("Assets/Title Screen/TitleScreenBackground.png");
	gameTitle= app->tex->Load("Assets/textures/test.png");
	return true;
}

bool TitleScreen::Update() {
	app->transition->TransitionStep(nullptr, this, true);
	return true;
}

// Update: draw background
bool TitleScreen::PostUpdate() {
	// Draw everything --------------------------------------
	app->render->DrawTexture(backgroundTexture, 0, 0, false);

	return true;
}

bool TitleScreen::CleanUp() {
	app->tex->UnLoad(backgroundTexture);

	return true;
}