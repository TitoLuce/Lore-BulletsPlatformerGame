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
#include "ModuleFonts.h"
//#include "TitleScreen.h"
#include "Transition.h"

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
	menuBackgroundTexture = app->tex->Load("Assets/menu_background2.png");

	respawn = true;

	btnResume = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "Resume", { 0, 0, 189, 44 }, this);
	btnSettings = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Settings", { 0, 0, 189, 44 }, this);
	btnBack2Title = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "TitleScreen", { 0, 0, 189, 44 }, this);
	btnQuit = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Quit", { 0, 0, 189, 44 }, this);
	btnBack = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "Back", { 0, 0, 189, 44 }, this);

	sldMusicVolume = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 6, "Music Volume:", { 0, 0, 40, 40 }, this, { 220, 250, 400, 40 });
	sldFxVolume = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 7, "Fx Volume:", { 0, 0, 40, 40 }, this, { 660, 250, 400, 40 });

	cbFullscreen = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 8, "Fullscreen", { 0, 400, 40, 40 }, this);
	cbVSync = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 9, "VSync", { 0, 0, 40, 40 }, this);



	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	app->render->camera.x = -player->playerRect.x + 600;
	app->render->camera.y = -player->playerRect.y + 300;

	btnResume->bounds.x = app->render->camera.x - 650;
	btnResume->bounds.y = app->render->camera.y - 450;

	btnSettings->bounds.x = app->render->camera.x - 650;
	btnSettings->bounds.y = app->render->camera.y - 350;

	btnBack2Title->bounds.x = app->render->camera.x - 650;
	btnBack2Title->bounds.y = app->render->camera.y - 250;

	btnQuit->bounds.x = app->render->camera.x - 650;
	btnQuit->bounds.y = app->render->camera.y - 150;

	btnBack->bounds.x = app->render->camera.x - 650;
	btnBack->bounds.y = app->render->camera.y - 150;

	sldMusicVolume->bounds.x = app->render->camera.x - 975;
	sldMusicVolume->bounds.y = app->render->camera.y - 450;
	sldMusicVolume->sliderBounds.x = app->render->camera.x - 975;
	sldMusicVolume->sliderBounds.y = app->render->camera.y - 450;

	sldFxVolume->bounds.x = app->render->camera.x - 525;
	sldFxVolume->bounds.y = app->render->camera.y - 450;
	sldFxVolume->sliderBounds.x = app->render->camera.x - 525;
	sldFxVolume->sliderBounds.y = app->render->camera.y - 450;

	cbFullscreen->bounds.x = app->render->camera.x - 700;
	cbFullscreen->bounds.y = app->render->camera.y - 250;

	cbVSync->bounds.x = app->render->camera.x - 300;
	cbVSync->bounds.y = app->render->camera.y - 250;

	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
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

	if (player->heDed == true) { app->render->DrawTexture(deathScreenTexture, -(app->render->camera.x - 200), -(app->render->camera.y - 250), nullptr); }
	if (menuOn || settingsOn)app->render->DrawTexture(menuBackgroundTexture, app->render->camera.x - 1100, app->render->camera.y - 540, false);
	if (menuOn && !settingsOn)
	{
		btnResume->Draw();
		btnSettings->Draw();
		btnBack2Title->Draw();
		btnQuit->Draw();
	}
	if (settingsOn)
	{
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
	case 10://Resume
	{
		menuOn = false;
	} break;
	case 11://Title Screen
	{
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