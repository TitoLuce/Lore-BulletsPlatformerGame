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
#include "Enemy.h"
#include "EnemySlime.h"
#include "EnemyFly.h"

#include "Input.h"

#include "SDL/include/SDL_scancode.h"

EnemyHandler::EnemyHandler() 
{ 
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
	name.Create("enemyHandler"); 
}

EnemyHandler::~EnemyHandler() {}


void EnemyHandler::Init()
{
	active = false;
}



// Load assets
bool EnemyHandler::Start()
{
	slimeTexture = app->tex->Load("Assets/Enemies/slime_sprites.png");
	flyTexture = app->tex->Load("Assets/Enemies/fly_sprites.png");
	//goblinTexture = app->tex->Load("Assets/Enemies/goblin_sprites.png");

	

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
	LOG("Loading Enemies");
	bool ret = true;
	
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

	return ret;
}

bool EnemyHandler::Update(float dt)
{
	
	HandleEnemiesSpawn();

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			enemies[i]->Update(dt);
		}
	}

	HandleEnemiesDespawn();

	////Slime Logic
	//if (slimeIsDed)
	//{
	//	currentSlimeAnimation = &slimeDed;
	//}
	//else
	//{
	//	currentSlimeAnimation = &slimeIdle;
	//}

	////Fly Logic
	//if (flyIsDed)
	//{
	//	currentFlyAnimation = &flyDed;
	//}
	//else
	//{
	//	currentFlyAnimation = &flyIdleOrMoving;
	//}

	////Goblin Logic
	//if (goblinIsDed)
	//{
	//	currentGoblinAnimation = &goblinDed;
	//}
	//else if(goblinIsHit)
	//{
	//	currentGoblinAnimation = &goblinHit;
	//	if (goblinCounter < 60) { goblinCounter++; }
	//	else
	//	{
	//		goblinIsHit = false;
	//		goblinCounter = 0;
	//	}
	//}
	//else
	//{
	//	currentGoblinAnimation = &goblinIdle;
	//}

	return true;
}

bool EnemyHandler::PostUpdate()
{

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
			enemies[i]->Draw();
	}

	//// Draw everything --------------------------------------
	//currentSlimeAnimation->Update();
	//app->render->DrawTexture(slimeTexture, slimeRect.x, slimeRect.y, &currentSlimeAnimation->GetCurrentFrame(), slimeInverted);

	//currentFlyAnimation->Update();
	//app->render->DrawTexture(flyTexture, flyRect.x, flyRect.y, &currentFlyAnimation->GetCurrentFrame(), flyInverted);

	//currentGoblinAnimation->Update();
	//app->render->DrawTexture(goblinTexture, goblinRect.x, goblinRect.y, &currentGoblinAnimation->GetCurrentFrame(), goblinInverted);

	return true;
}

bool EnemyHandler::CleanUp()
{
	/*app->tex->UnLoad(slimeTexture);
	app->tex->UnLoad(flyTexture);
	app->tex->UnLoad(goblinTexture);*/
	LOG("Freeing all enemies");

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}
	return true;
}



bool EnemyHandler::Load(pugi::xml_node& save)
{
	LOG("Loading enemy data");
	bool ret = true;
	int i = 0;

	int x = 0;
	int y = 0;
	EnemyType type = EnemyType::NO_TYPE;
	bool destroyed = false;

	for (pugi::xml_node enemy = save.child("enemy"); enemy && ret; enemy = enemy.next_sibling("enemy"))
	{
		destroyed = enemy.child("destroyed").attribute("value").as_bool();
		if (destroyed == true && enemies[i] != nullptr)
		{
			enemies[i]->pendingToDelete = true;
		}
		else
		{
			x = enemy.child("coordinates").attribute("x").as_int();
			y = enemy.child("coordinates").attribute("y").as_int();
			switch (enemy.child("type").attribute("value").as_int())
			{
			case 1:
				type = EnemyType::SLIME;
				break;
			case 2:
				type = EnemyType::FLY;
				break;
			default:
				type = EnemyType::NO_TYPE;
				break;
			}

			if (enemies[i] == nullptr)
			{
				AddEnemy(type, x, y, i);
			}
			else
			{
				enemies[i]->enemyRect.x = x;
				enemies[i]->enemyRect.y = y;
			}
		}
		i++;
	}

	return ret;
}

bool EnemyHandler::Save(pugi::xml_node& save)
{
	LOG("Saving enemy data");
	bool ret = true;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		pugi::xml_node enemy = save.append_child("enemy");
		if (enemies[i] != nullptr)
		{
			pugi::xml_node enemyCoords = enemy.append_child("coordinates");
			enemyCoords.append_attribute("x").set_value(enemies[i]->enemyRect.x);
			enemyCoords.append_attribute("y").set_value(enemies[i]->enemyRect.y);
			//enemy.append_child("type").append_attribute("value").set_value(enemies[i]->type);
			enemy.append_child("destroyed").append_attribute("value").set_value(enemies[i]->pendingToDelete);
		}
		else
		{
			enemy.append_child("destroyed").append_attribute("value").set_value(true);
		}
	}
	return ret;
}

bool EnemyHandler::AddEnemy(EnemyType type, int x, int y, uint i)
{
	bool ret = false;
	if (i == -1)
	{
		for (i = 0; i < MAX_ENEMIES; ++i)
		{
			if (spawnQueue[i].type == EnemyType::NO_TYPE)
			{
				spawnQueue[i].type = type;
				spawnQueue[i].x = x;
				spawnQueue[i].y = y;
				ret = true;
				break;
			}
		}
	}
	else
	{
		if (spawnQueue[i].type == EnemyType::NO_TYPE)
		{
			spawnQueue[i].type = type;
			spawnQueue[i].x = x;
			spawnQueue[i].y = y;
			ret = true;
		}
	}

	return ret;
}

void EnemyHandler::HandleEnemiesSpawn()
{
	// Iterate all the enemies queue
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (spawnQueue[i].type != EnemyType::NO_TYPE)
		{
			LOG("Spawning enemy at %d", spawnQueue[i].x * app->win->GetScale());
			SpawnEnemy(spawnQueue[i]);
			spawnQueue[i].type = EnemyType::NO_TYPE; // Removing the newly spawned enemy from the queue
		}
	}
}

void EnemyHandler::HandleEnemiesDespawn()
{
	// Iterate existing enemies
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			// Delete the enemy when it has reached the end of the screen
			if (enemies[i]->pendingToDelete)
			{
				LOG("DeSpawning enemy at %d", enemies[i]->enemyRect.x * app->win->GetScale());

				delete enemies[i];
				enemies[i] = nullptr;
			}
		}
	}
}

void EnemyHandler::SpawnEnemy(const EnemySpawnpoint& info)
{
	// Find an empty slot in the enemies array
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] == nullptr)
		{
			switch (info.type)
			{
			case EnemyType::SLIME:
				enemies[i] = new EnemySlime(info.x, info.y, info.type);
				enemies[i]->texture = slimeTexture;
				enemies[i]->enemyPhysics.verlet = true;
				break;
			case EnemyType::FLY:
				enemies[i] = new EnemyFly(info.x, info.y, info.type);
				enemies[i]->texture = flyTexture;
				enemies[i]->enemyPhysics.verlet = false;
				break;
			}
			enemies[i]->enemyPhysics.axisX = true;
			enemies[i]->enemyPhysics.axisY = true;
			enemies[i]->destroyedFx = hitSFX;
			app->audio->PlayFx(deathSFX, 40, 0);
			break;
		}
	}
}

void EnemyHandler::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->collider == c1)
		{
			enemies[i]->OnCollision(c1, c2); //Notify the enemy of a collision
		}
	}
}