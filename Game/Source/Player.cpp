#include "Player.h"

#include "App.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "Transition.h"
#include "Scene.h"

#include "Input.h"

#include "SDL/include/SDL_scancode.h"

Player::Player() : Module() {
	name.Create("titleScreen");

}

Player::~Player() {}

// Load assets
bool Player::Start() {
	app->transition->TransitionStep(nullptr, this, true, 1200.0f);
	playerTexture = app->tex->Load("Assets/PlayerSprites.png");
	return true;
}

bool Player::PreUpdate()
{

	return true;
}

bool Player::Update(float dt) {
		playerRect = { 0, 256, 64, 64 };
	return true;
}

// Update: draw background
bool Player::PostUpdate() {
	// Draw everything --------------------------------------
	app->render->DrawTexture(playerTexture, 0, 0, &playerRect);
	return true;
}

bool Player::CleanUp() {
	app->tex->UnLoad(playerTexture);
	return true;
}


void Player::Init()
{
	active = false;
}