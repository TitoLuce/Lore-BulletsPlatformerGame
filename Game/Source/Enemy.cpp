#include "Enemy.h"

#include "App.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "Transition.h"
#include "Scene.h"
#include "Map.h"
#include "Defs.h"
#include "Log.h"

#include "Input.h"

#include "SDL/include/SDL_scancode.h"

Enemy::Enemy()
{
	name.Create("enemy");
}

Enemy::~Enemy() {}

// Load assets
bool Enemy::Start()
{
	slimeTexture = app->tex->Load("Assets/Enemies/SlimeSprites.png");

	slimeRect = { 1600 , 5100 , slimeIdle.GetCurrentFrame().w, slimeIdle.GetCurrentFrame().h };

	//Reset animations
	slimeMoving.Reset();

	slimeIsDed = false;
	slimeInverted = false;
	return true;
}

bool Enemy::Awake(pugi::xml_node&)
{
	//Slime Animations
	for (int i = 0; i < 5; i++)
	{
		slimeIdle.PushBack({ 0 + i * 64,128,64,48 });
	}
	slimeIdle.SetSpeed(0.14f);

	for (int i = 0; i < 7; i++)
	{
		slimeMoving.PushBack({ 0 + i * 64,0,64,48 });
	}
	slimeMoving.SetSpeed(0.14f);

	for (int i = 0; i < 7; i++)
	{
		slimeDed.PushBack({ 0 + i * 64,60,64,48 });
	}
	slimeDed.SetSpeed(0.14f);
	slimeDed.SetLoop(false);

	return true;
}

bool Enemy::PreUpdate() { return true; }

bool Enemy::Update(float dt)
{
	//Pathfinding has to be done here lol

	//Slime Logic
	if (slimeIsDed)
	{
		currentSlimeAnimation = &slimeDed;
	}
	else
	{
		currentSlimeAnimation = &slimeIdle;
	}

	return true;
}

bool Enemy::PostUpdate()
{
	// Draw everything --------------------------------------
	currentSlimeAnimation->Update();
	app->render->DrawTexture(slimeTexture, slimeRect.x, slimeRect.y, &currentSlimeAnimation->GetCurrentFrame(), slimeInverted);
	return true;
}

bool Enemy::CleanUp()
{
	app->tex->UnLoad(slimeTexture);
	return true;
}
