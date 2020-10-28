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
	idle.SetSpeed(0.04f);

	moving.PushBack({ 0,64,64,64 });
	moving.PushBack({ 64,64,64,64 });
	moving.PushBack({ 128,64,64,64 });
	moving.PushBack({ 192,64,64,64 });
	moving.PushBack({ 256,64,64,64 });
	moving.PushBack({ 320,64,64,64 });
	moving.SetSpeed(0.05f);

	jumping.PushBack({ 0,384,64,64 });
	jumping.PushBack({ 64,384,64,64 });
	jumping.PushBack({ 128,384,64,64 });
	jumping.SetSpeed(0.03f);

	doubleJumping.PushBack({ 0,576,64,64 });
	doubleJumping.PushBack({ 64,576,64,64 });
	doubleJumping.PushBack({ 128,576,64,64 });
	doubleJumping.SetSpeed(0.03f);

	ded.PushBack({ 0,0,64,64 });
	ded.PushBack({ 64,0,64,64 });
	ded.PushBack({ 128,0,64,64 });
	ded.PushBack({ 192,0,64,64 });
	ded.PushBack({ 256,0,64,64 });
	ded.PushBack({ 320,0,64,64 });
	ded.PushBack({ 384,0,64,64 });
	ded.SetSpeed(0.03f);

	jumpDown.PushBack({ 0,320,64,64 });
	jumpDown.PushBack({ 64,320,64,64 });
	jumpDown.PushBack({ 128,320,64,64 });
	jumpDown.SetSpeed(0.03f);
	
}

Player::~Player() {}

// Load assets
bool Player::Start() {
	app->transition->TransitionStep(nullptr, this, true, 1200.0f);
	playerTexture = app->tex->Load("Assets/PlayerSprites.png");

	//Reset animations
	moving.Reset();
	ded.Reset();

	speedX = 3.0f;

	heDed = false;
	inverted = false;
	return true;
}

bool Player::Update(float dt) {
	if (!heDed)
	{
		if (speedY <= 5.0f)
		{
			speedY -= 0.05f;
		}
		
		if (speedY <= 0) 
		{
			currentAnimation = &idle;
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
		{

			inverted = false;

			if (app->input->GetKey(SDL_SCANCODE_SPACE) != KEY_REPEAT)
			{
				currentAnimation = &moving;
			}
			
			x += speedX;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
		{
			
			if (!inverted)
			{
				inverted = true;
			}

			if (app->input->GetKey(SDL_SCANCODE_SPACE) != KEY_REPEAT)
			{
				currentAnimation = &moving;
			}
			x -= speedX;
		}
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT)
		{
			currentAnimation = &jumping;
			if (jumps == 2)
			{
				speedY = 2.0f;
			}
			if (jumps == 1)
			{
				speedY = 1.5f;
			}
			
			y -= speedY;
		}
		
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT /*&& speedY > 0*/)
		{
			currentAnimation = &jumpDown;
		}
	}
	return true;
}

// Update: draw background
bool Player::PostUpdate() {
	// Draw everything --------------------------------------
	currentAnimation->Update();
	app->render->DrawTexture(playerTexture, x, y, &currentAnimation->GetCurrentFrame(), inverted);
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