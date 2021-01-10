#include "EntityManager.h"

#include "App.h"
#include "Render.h"
#include "Audio.h"
#include "Textures.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyFly.h"
#include "EnemySlime.h"
//#include "Item.h"
#include "Transition.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	return ret;
}

bool EntityManager::Start()
{
	//app->transition->TransitionStep(nullptr, this, true, 1200.0f);
	//load all the the images and audios here

	//textures
	flyTexture = app->tex->Load("Assets/Enemies/fly_sprites.png");
	slimeTexture = app->tex->Load("Assets/Enemies/slime_sprites.png");
	playerTexture = app->tex->Load("Assets/player_sprites.png");
	specialBarTexture = app->tex->Load("Assets/special_bar.png");

	//fx's
	jumpSFX = app->audio->LoadFx("Assets/Audio/Fx/jump_one.wav");
	doubleJumpSFX = app->audio->LoadFx("Assets/Audio/Fx/jump_two.wav");
	deathSFX = app->audio->LoadFx("Assets/Audio/Fx/death.wav");
	coinSFX = app->audio->LoadFx("Assets/Audio/Fx/coin.wav");
	attackSFX = app->audio->LoadFx("Assets/Audio/Fx/attack.wav");
	specialSFX = app->audio->LoadFx("Assets/Audio/Fx/special.wav");
	flagSFX = app->audio->LoadFx("Assets/Audio/Fx/checkpoint.wav");


	doLogic = true;

	return true;
}

// Called before quitting
bool EntityManager::CleanUp()
{

	// Destroy entities
	ListItem<Entity*>* e = entities.start;
	while (e != nullptr)
	{
		ListItem<Entity*>* eNext = e->next;
		DestroyEntity(e->data);
		e = eNext;
	}
	entities.Clear();


	app->tex->UnLoad(playerTexture);
	app->tex->UnLoad(flyTexture);
	app->tex->UnLoad(slimeTexture);
	app->tex->UnLoad(specialBarTexture);

	app->audio->UnloadFx(jumpSFX);
	app->audio->UnloadFx(attackSFX);
	app->audio->UnloadFx(coinSFX);
	app->audio->UnloadFx(deathSFX);
	app->audio->UnloadFx(doubleJumpSFX);
	app->audio->UnloadFx(flagSFX);
	app->audio->UnloadFx(specialSFX);


	return true;
}

Entity* EntityManager::CreateEntity(int x, int y, EntityType type, Entity* playerPointer, EnemyType eType)
{
	Entity* ret = nullptr;



	switch (type)
	{
	case EntityType::PLAYER:
		ret = new Player(x, y);
		break;
	case EntityType::ENEMY:

		switch (eType)
		{
		case EnemyType::FLYING:

			ret = new EnemyFly(x, y, eType, playerPointer);
			break;
		case EnemyType::GROUND:

			ret = new EnemySlime(x, y, eType, playerPointer);
			break;
		default:
			break;
		}
		break;
		/*case EntityType::ITEM:
			break;*/
	default:
		break;
	}

	// Created entities are added to the list
	if (ret != nullptr) entities.Add(ret);

	return ret;
}



bool EntityManager::Update(float dt)
{

	UpdateAll(dt, doLogic);


	return true;
}

bool EntityManager::UpdateAll(float dt, bool doLogic)
{
	if (doLogic)
	{
		//Update all entities 

		app->collisions->PreUpdate();
		ListItem<Entity*>* e = entities.start;
		while (e != nullptr)
		{
			e->data->Update(dt);
			e = e->next;
		}
	}

	return true;
}


bool EntityManager::PostUpdate()
{
	ListItem<Entity*>* e = entities.start;
	while (e != nullptr)
	{
		if (e->data->pendingToDelete == true)
		{
			DestroyEntity(e->data);
		}
		else
		{
			e->data->Draw();
		}
		e = e->next;
	}



	return true;
}


void EntityManager::DestroyEntity(Entity* entity)
{
	if (entity->collider != nullptr)
	{
		entity->collider->pendingToDelete = true;
	}
	int i = entities.Find(entity);
	delete entities[i];
	entities.Del(entities.At(i));
}


void EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	for (int i = 0; i < entities.Count(); i++)
	{
		if (c1 == entities[i]->collider)
		{
			entities[i]->OnCollision(c1, c2);
		}
	}
}


bool EntityManager::Load(pugi::xml_node& save)
{
	LOG("Loading entities data");
	bool ret = true;

	// Clear the list
	ListItem<Entity*>* e = entities.start;
	while (e != nullptr)
	{
		ListItem<Entity*>* eNext = e->next;
		DestroyEntity(e->data);
		e = eNext;
	}
	entities.Clear();

	// Initialize the entity variables
	int x = 0;
	int y = 0;
	EntityType type = EntityType::UNKNOWN;
	EnemyType eType = EnemyType::NO_TYPE;
	Player* pp = nullptr;

	for (pugi::xml_node entity = save.child("entity"); entity && ret; entity = entity.next_sibling("entity"))
	{
		x = entity.child("coordinates").attribute("x").as_int();
		y = entity.child("coordinates").attribute("y").as_int();
		switch (entity.child("type").attribute("value").as_int())
		{
		case 0:
			type = EntityType::PLAYER;
			break;
		case 1:
			type = EntityType::ENEMY;
			break;
		default:
			type = EntityType::UNKNOWN;
			break;
		}
		switch (entity.child("eType").attribute("value").as_int())
		{
		case 1:
			eType = EnemyType::GROUND;
			break;
		case 2:
			eType = EnemyType::FLYING;
			break;
		default:
			eType = EnemyType::NO_TYPE;
			break;
		}

		if (type == EntityType::PLAYER)
		{
			pp = (Player*)CreateEntity(x, y, type, nullptr, eType);
		}
		else
		{
			CreateEntity(x, y, type, pp, eType);
		}
	}
	return ret;
}

bool EntityManager::Save(pugi::xml_node& save)
{
	LOG("Saving entities data");
	bool ret = true;

	ListItem<Entity*>* e = entities.start;
	while (e != nullptr)
	{
		pugi::xml_node entity = save.append_child("entity");
		pugi::xml_node entityCoords = entity.append_child("coordinates");
		entityCoords.append_attribute("x").set_value(e->data->entityRect.x);
		entityCoords.append_attribute("y").set_value(e->data->entityRect.y);
		int type = 0;
		switch (e->data->type)
		{
		case EntityType::PLAYER:
		{
			Player* pp = (Player*)e->data;
			type = 0;
			break;
		}
		case EntityType::ENEMY:
		{
			type = 1;
			break;
		}
		default:
		{
			break;
		}
		}
		entity.append_child("type").append_attribute("value").set_value(type);
		int eType = 0;
		switch (e->data->eType)
		{
		case EnemyType::GROUND:
			eType = 1;
			break;
		case EnemyType::FLYING:
			eType = 2;
			break;
		default:
			break;
		}
		entity.append_child("eType").append_attribute("value").set_value(eType);

		e = e->next;
	}
	return ret;
}