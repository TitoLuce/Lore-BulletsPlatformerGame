#include "EnemyHandler.h"

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

EnemyHandler::EnemyHandler() { name.Create("enemyHandler"); }

EnemyHandler::~EnemyHandler() {}


void EnemyHandler::Init()
{
	active = false;
}



// Load assets
bool EnemyHandler::Start()
{
	slimeTexture = app->tex->Load("Assets/Enemies/SlimeSprites.png");
	flyTexture = app->tex->Load("Assets/Enemies/FlySprites.png");
	goblinTexture = app->tex->Load("Assets/Enemies/GoblinSprites.png");

	hitSFX = app->audio->LoadFx("Assets/Audio/fx/Hit.wav");
	deathSFX = app->audio->LoadFx("Assets/Audio/fx/EnemyDeath.wav");

	//slimeRect = { 1600 , 5100 , slimeIdle.GetCurrentFrame().w, slimeIdle.GetCurrentFrame().h };
	//flyRect = { 1680 , 5100 , flyIdleOrMoving.GetCurrentFrame().w, flyIdleOrMoving.GetCurrentFrame().h };
	//goblinRect = { 1760 , 5100 , goblinIdle.GetCurrentFrame().w, goblinIdle.GetCurrentFrame().h };

	////Reset animations
	//slimeMoving.Reset();
	//flyIdleOrMoving.Reset();
	//goblinMoving.Reset();

	//slimeIsDed = false;
	//slimeInverted = false;

	//flyIsDed = false;
	//flyInverted = false;

	//goblinIsDed = false;
	//goblinHP = 2;
	//goblinIsHit = false;
	//int goblinCounter = 0;
	//goblinInverted = false;

	return true;
}

bool EnemyHandler::Awake(pugi::xml_node&)
{
	////Slime Animations
	//for (int i = 0; i < 5; i++)
	//{
	//	slimeIdle.PushBack({ i * 64,128,64,48 });
	//}
	//slimeIdle.SetSpeed(0.14f);

	//for (int i = 0; i < 7; i++)
	//{
	//	slimeMoving.PushBack({ i * 64,0,64,48 });
	//}
	//slimeMoving.SetSpeed(0.14f);

	//for (int i = 0; i < 7; i++)
	//{
	//	slimeDed.PushBack({ i * 64,60,64,48 });
	//}
	//slimeDed.SetSpeed(0.14f);
	//slimeDed.SetLoop(false);

	////Fly animations
	//for (int i = 0; i < 3; i++)
	//{
	//	flyIdleOrMoving.PushBack({ i * 62,70,62,54 });
	//}
	//flyIdleOrMoving.SetSpeed(0.08f);

	//for (int i = 0; i < 5; i++)
	//{
	//	flyDed.PushBack({ i * 62,0,62,54 });
	//}
	//flyDed.PushBack({ 0,0,1,1 });
	//flyDed.SetSpeed(0.14f);
	//flyDed.SetLoop(false);

	////Goblin Animations
	//for (int i = 0; i < 4; i++)
	//{
	//	goblinIdle.PushBack({ i * 64,124,64,64 });
	//}
	//goblinIdle.SetSpeed(0.14f);

	//for (int i = 0; i < 6; i++)
	//{
	//	goblinMoving.PushBack({ i * 64,0,64,64 });
	//}
	//goblinMoving.SetSpeed(0.14f);

	//for (int i = 0; i < 3; i++)
	//{
	//	goblinHit.PushBack({ i * 64,188,64,64 });
	//}
	//goblinHit.SetSpeed(0.12f);

	//for (int i = 0; i < 3; i++)
	//{
	//	goblinDed.PushBack({ i * 64,64,64,60 });
	//}
	//goblinDed.SetSpeed(0.14f);
	//goblinDed.SetLoop(false);

	return true;
}

bool EnemyHandler::Update(float dt)
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

	//Fly Logic
	if (flyIsDed)
	{
		currentFlyAnimation = &flyDed;
	}
	else
	{
		currentFlyAnimation = &flyIdleOrMoving;
	}

	//Goblin Logic
	if (goblinIsDed)
	{
		currentGoblinAnimation = &goblinDed;
	}
	else if(goblinIsHit)
	{
		currentGoblinAnimation = &goblinHit;
		if (goblinCounter < 60) { goblinCounter++; }
		else
		{
			goblinIsHit = false;
			goblinCounter = 0;
		}
	}
	else
	{
		currentGoblinAnimation = &goblinIdle;
	}

	return true;
}

bool EnemyHandler::PostUpdate()
{
	// Draw everything --------------------------------------
	currentSlimeAnimation->Update();
	app->render->DrawTexture(slimeTexture, slimeRect.x, slimeRect.y, &currentSlimeAnimation->GetCurrentFrame(), slimeInverted);

	currentFlyAnimation->Update();
	app->render->DrawTexture(flyTexture, flyRect.x, flyRect.y, &currentFlyAnimation->GetCurrentFrame(), flyInverted);

	currentGoblinAnimation->Update();
	app->render->DrawTexture(goblinTexture, goblinRect.x, goblinRect.y, &currentGoblinAnimation->GetCurrentFrame(), goblinInverted);

	return true;
}

bool EnemyHandler::CleanUp()
{
	app->tex->UnLoad(slimeTexture);
	app->tex->UnLoad(flyTexture);
	app->tex->UnLoad(goblinTexture);

	return true;
}
