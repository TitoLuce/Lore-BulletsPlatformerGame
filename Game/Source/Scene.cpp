#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

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
	//app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
	app->render->camera.x = -(app->player->spawnpointX - app->player->playerRect.x - 640);
	app->render->camera.y = -(app->player->spawnpointY - app->player->playerRect.y - 310);
	app->map->Enable();
	app->map->Load("Level_1.tmx");
	app->player->Enable();
	deathScreenTexture = app->tex->Load("Assets/DeathScreen.png");
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if((app->render->camera.x + app->player->playerRect.x) < (app->map->data.tileWidth*10))
	{
		app->render->camera.x += 5;
	}
	if ((app->player->playerRect.w + app->render->camera.x + app->player->playerRect.x) > (app->render->camera.w - app->map->data.tileWidth*10))
	{
		app->render->camera.x -= 5;
	}
	if ((app->render->camera.y + app->player->playerRect.y) < (app->map->data.tileHeight*6))
	{
		app->render->camera.y += 5;
	}
	if ((app->player->playerRect.h + app->render->camera.y + app->player->playerRect.y) > (app->render->camera.h - app->map->data.tileHeight*6))
	{
		app->render->camera.y -= 5;
	}

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadRequest();

	app->map->Draw();
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;
	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;  //DO NOT REMOVE EXIT GAME WITH ESC

	if (app->player->heDed == true)
	{
		app->render->DrawTexture(deathScreenTexture, app->player->playerRect.x - 450, app->player->playerRect.y - 155/*app->render->camera.y*/, nullptr);
	}
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


void Scene::Init()
{
	active = false;
}