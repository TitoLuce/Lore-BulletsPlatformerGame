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
	idle.PushBack({ 0,256,64,64 });
	idle.PushBack({ 64,256,64,64 });
	idle.PushBack({ 128,256,64,64 });
	idle.PushBack({ 192,256,64,64 });

	moving.PushBack({ 0,64,64,64 });
	moving.PushBack({ 64,64,64,64 });
	moving.PushBack({ 128,64,64,64 });
	moving.PushBack({ 192,64,64,64 });
	moving.PushBack({ 256,64,64,64 });
	moving.PushBack({ 320,64,64,64 });

	jumping.PushBack({ 0,384,64,64 });
	jumping.PushBack({ 64,384,64,64 });
	jumping.PushBack({ 128,384,64,64 });

	doubleJumping.PushBack({ 0,576,64,64 });
	doubleJumping.PushBack({ 64,576,64,64 });
	doubleJumping.PushBack({ 128,576,64,64 });

	ded.PushBack({ 0,0,64,64 });
	ded.PushBack({ 64,0,64,64 });
	ded.PushBack({ 128,0,64,64 });
	ded.PushBack({ 192,0,64,64 });
	ded.PushBack({ 256,0,64,64 });
	ded.PushBack({ 320,0,64,64 });
	ded.PushBack({ 384,0,64,64 });

	jumpDown.PushBack({ 0,320,64,64 });
	jumpDown.PushBack({ 64,320,64,64 });
	jumpDown.PushBack({ 128,320,64,64 });
}

Player::~Player() {}

// Load assets
bool Player::Start() {
	app->transition->TransitionStep(nullptr, this, true, 1200.0f);
	playerTexture = app->tex->Load("Assets/PlayerSprites.png");

	//Reset animations
	moving.Reset();
	ded.Reset();
	return true;
}

bool Player::Update(float dt) {
		
	if (!heDed)
	{

	}
	return true;
}

// Update: draw background
bool Player::PostUpdate() {
	// Draw everything --------------------------------------
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