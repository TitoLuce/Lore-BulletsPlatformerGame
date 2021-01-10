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
#include "Transition.h"
#include "Animation.h"
#include "ModuleFonts.h"

#include "TitleScreen.h"

#include "EntityManager.h"
#include "GuiManager.h"


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

	app->collisions->Enable();
	app->entityManager->Enable();

	player = (Player*)app->entityManager->CreateEntity(1600, 5120, EntityType::PLAYER);

	app->audio->PlayMusic("Assets/Audio/Music/child's_nightmare.ogg");
	app->render->camera.x = -(player->spawnpointX - player->entityRect.x /*+ 1600*/);
	app->render->camera.y = -(player->spawnpointY - player->entityRect.y /*+ 5120*/);
	
	app->map->Load("level_1.tmx");
	//player->Enable();

	app->entityManager->CreateEntity(app->map->data.tileWidth * 27, app->map->data.tileHeight * 74, EntityType::ENEMY, player, EnemyType::FLYING);
	app->entityManager->CreateEntity(app->map->data.tileWidth * 44, app->map->data.tileHeight * 87, EntityType::ENEMY, player, EnemyType::GROUND);


	app->map->Enable();
	if (app->map->Load("level_1.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;

		if (app->map->CreateWalkabilityMap(&w, &h, &data))
		{
			app->pathfinding->SetMap(w, h, data);
		}

		RELEASE_ARRAY(data);
	}

	deathScreenTexture = app->tex->Load("Assets/death_screen.png");
	menuBackgroundTexture = app->tex->Load("Assets/menu_background2.png");

	respawn = true;
	menuOn = false;
	settingsOn = false;

	btnResume = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "Resume", { 0, 0, 189, 44 }, this);
	btnSettings = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Settings", { 0, 0, 189, 44 }, this);
	btnBack2Title = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "TitleScreen", { 0, 0, 189, 44 }, this);
	btnQuit = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Quit", { 0, 0, 189, 44 }, this);
	btnBack = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "Back", { 0, 0, 189, 44 }, this);

	sldMusicVolume = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 6, "Music Volume:", { 0, 0, 40, 40 }, this, { 220, 250, 400, 40 });
	sldFxVolume = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 7, "Fx Volume:", { 0, 0, 40, 40 }, this, { 660, 250, 400, 40 });

	cbFullscreen = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 8, "Fullscreen", { 0, 400, 40, 40 }, this);
	cbVSync = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 9, "VSync", { 0, 0, 40, 40 }, this);

	seconds = 0;
	minutes = 0;

	if (app->titleScreen->continueOn)
	{
		app->titleScreen->continueOn = false;
		app->LoadRequest();
	}

	cameraPos = { -app->render->camera.x, -app->render->camera.y };

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{

	app->render->camera.x = -player->entityRect.x + 600;
	app->render->camera.y = -player->entityRect.y + 300;

	cameraPos = { -app->render->camera.x, -app->render->camera.y };

	btnResume->bounds.x = cameraPos.x + 550;
	btnResume->bounds.y = cameraPos.y + 175;

	btnSettings->bounds.x = cameraPos.x + 550;
	btnSettings->bounds.y = cameraPos.y + 275;

	btnBack2Title->bounds.x = cameraPos.x + 550;
	btnBack2Title->bounds.y = cameraPos.y + 375;

	btnQuit->bounds.x = cameraPos.x + 550;
	btnQuit->bounds.y = cameraPos.y + 475;

	btnBack->bounds.x = cameraPos.x + 550;
	btnBack->bounds.y = cameraPos.y + 475;


	if (!settingsOn)
	{
		sldMusicVolume->bounds.x = cameraPos.x + 240;
		sldMusicVolume->bounds.y = cameraPos.y + 200;
		sldMusicVolume->sliderBounds.x = cameraPos.x + 240;
		sldMusicVolume->sliderBounds.y = cameraPos.y + 200;

		sldFxVolume->bounds.x = cameraPos.x + 660;
		sldFxVolume->bounds.y = cameraPos.y + 200;
		sldFxVolume->sliderBounds.x = cameraPos.x + 660;
		sldFxVolume->sliderBounds.y = cameraPos.y + 200;
	}


	cbFullscreen->bounds.x = cameraPos.x + 510;
	cbFullscreen->bounds.y = cameraPos.y + 350;

	cbVSync->bounds.x = cameraPos.x + 900;
	cbVSync->bounds.y = cameraPos.y + 350;

	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{


	ListItem<Entity*>* e = app->entityManager->entities.start;
	while (e != nullptr)
	{
		if (e->data->type == EntityType::PLAYER)
		{
			player = (Player*)e->data;
		}
		e = e->next;
	}

	if (!menuOn && !settingsOn) { seconds += dt; }
	if(seconds>=60)
	{
		minutes++;
		seconds = 0;
	}
	if (minutes >= 99) { minutes = 99; }

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) menuOn = true; //Needs to pause entities and timer too
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) { app->SaveRequest(); }
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) { app->LoadRequest(); }

	app->map->Draw();

	btnResume->Update(dt);
	btnSettings->Update(dt);
	btnBack2Title->Update(dt);
	btnQuit->Update(dt);
	btnBack->Update(dt);

	sldMusicVolume->Update(dt);
	sldFxVolume->Update(dt);
	cbFullscreen->Update(dt);
	cbVSync->Update(dt);

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	if (exit) { return false; }

	app->fonts->BlitText(cameraPos.x + 10, cameraPos.y + 10, app->titleScreen->font, "TIME:");
	sprintf_s(timer, 8, "%02d :%02d", (int)minutes, (int) seconds);
	app->fonts->BlitText(cameraPos.x + 170, cameraPos.y + 10, app->titleScreen->font, timer);

	if (player->heDed == true) { app->render->DrawTexture(deathScreenTexture, -(app->render->camera.x - 200), -(app->render->camera.y - 250), nullptr); }
	if (menuOn || settingsOn)app->render->DrawTexture(menuBackgroundTexture, cameraPos.x + 100, cameraPos.y + 75, false);
	if (menuOn && !settingsOn)
	{
		btnResume->Draw();
		btnSettings->Draw();
		btnBack2Title->Draw();
		btnQuit->Draw();
	}
	if (settingsOn)
	{
		app->entityManager->doLogic = false;
		sldMusicVolume->Draw();
		sldFxVolume->Draw();
		cbFullscreen->Draw();
		cbVSync->Draw();
		btnBack->Draw();
	}

	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->map->Disable();
	app->tex->UnLoad(deathScreenTexture);
	app->fonts->Unload(app->titleScreen->font);
	app->fonts->Unload(app->titleScreen->font2);
	return true;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->id)
	{
	case 3://Settings
	{
		settingsOn = true;
		sldMusicVolume->state = GuiControlState::NORMAL;
		sldFxVolume->state = GuiControlState::NORMAL;
		cbFullscreen->state = GuiControlState::NORMAL;
		cbVSync->state = GuiControlState::NORMAL;
		btnResume->state = GuiControlState::DISABLED;
		btnSettings->state = GuiControlState::DISABLED;
		btnBack2Title->state = GuiControlState::DISABLED;
		btnQuit->state = GuiControlState::DISABLED;
	} break;
	case 5://Quit
	{
		exit = true;
	} break;
	case 8: //Toggle Fullscreen
	{
		app->win->ToggleFullscreen(cbFullscreen->checked);
	} break;
	case 10://Resume
	{
		menuOn = false;
	} break;
	case 11://Title Screen
	{
		app->entityManager->Disable();
		app->transition->TransitionStep(this, (Module*)app->titleScreen, false, 10.0f);
	} break;
	case 12://Back
	{
		settingsOn = false;
		sldMusicVolume->state = GuiControlState::DISABLED;
		sldFxVolume->state = GuiControlState::DISABLED;
		cbFullscreen->state = GuiControlState::DISABLED;
		cbVSync->state = GuiControlState::DISABLED;
		btnResume->state = GuiControlState::NORMAL;
		btnSettings->state = GuiControlState::NORMAL;
		btnBack2Title->state = GuiControlState::NORMAL;
		btnQuit->state = GuiControlState::NORMAL;
	} break;
	default:
		break;
	}
	return true;
}

void Scene::Init() { active = false; }