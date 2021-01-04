#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "PathFinding.h"
#include "EnemyHandler.h"
#include "EntityManager.h"


#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module() { name.Create("scene"); }

// Destructor
Scene::~Scene() {}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);

	app->audio->PlayMusic("Assets/Audio/Music/child's_nightmare.ogg");
	app->render->camera.x = -(player->spawnpointX - player->playerRect.x /*+ 1600*/);
	app->render->camera.y = -(player->spawnpointY - player->playerRect.y /*+ 5120*/);
	app->map->Enable();
	app->map->Load("level_1.tmx");
	//player->Enable();

	/*app->enemies->Enable();
	app->enemies->AddEnemy(EnemyType::FLY, app->map->data.tileWidth * 27, app->map->data.tileHeight * 74);
	app->enemies->AddEnemy(EnemyType::SLIME, app->map->data.tileWidth * 44, app->map->data.tileHeight * 87);*/

	app->collisions->Enable();
	

	app->map->Enable();
	if (app->map->Load("level_1.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;

		if (app->map->CreateWalkabilityMap(&w, &h, &data))
		{
			//app->pathfinding->SetMap(w, h, data);
		}

		RELEASE_ARRAY(data);
	}

	deathScreenTexture = app->tex->Load("Assets/death_screen.png");
	respawn = true;
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate() { return true; }

// Called each loop iteration
bool Scene::Update(float dt)
{
	app->render->camera.x = -player->playerRect.x + 600;
	app->render->camera.y = -player->playerRect.y + 300;

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) { app->SaveRequest(); }
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) { app->LoadRequest(); }

	app->map->Draw();
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;
	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;  //DO NOT REMOVE EXIT GAME WITH ESC

	if (player->heDed == true) { app->render->DrawTexture(deathScreenTexture, -(app->render->camera.x - 200), -(app->render->camera.y - 250), nullptr); }
	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->map->Disable();
	app->tex->UnLoad(deathScreenTexture);

	return true;
}

void Scene::Init() { active = false; }