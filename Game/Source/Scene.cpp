#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "PathFinding.h"
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
	app->render->camera.x = -(player->playerSpawnpointX - player->entityRect.x /*+ 1600*/);
	app->render->camera.y = -(player->playerSpawnpointY - player->entityRect.y /*+ 5120*/);
	
	app->map->Load("level_1.tmx");
	//player->Enable();

	fly = app->entityManager->CreateEntity(app->map->data.tileWidth * 27, app->map->data.tileHeight * 74, EntityType::ENEMY, player, EnemyType::FLYING);
	slime = app->entityManager->CreateEntity(app->map->data.tileWidth * 44, app->map->data.tileHeight * 87, EntityType::ENEMY, player, EnemyType::GROUND);


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

	btnResume = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "Resume", { 550, 175, 189, 44 }, this);
	btnSettings = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Settings", { 550, 275, 189, 44 }, this);
	btnBack2Title = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "TitleScreen", { 550, 375, 189, 44 }, this);
	btnQuit = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Quit", { 550, 475, 189, 44 }, this);
	btnBack = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "Back", { 550, 475, 189, 44 }, this);

	//sldMusicVolume = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 6, "Music Volume:", { 0, 0, 40, 40 }, this, { 220, 250, 400, 40 });
	//sldFxVolume = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 7, "Fx Volume:", { 0, 0, 40, 40 }, this, { 660, 250, 400, 40 });
	//sldMusicVolume->value = app->audio->GetVolumeMusic();
	//sldFxVolume->value = app->audio->GetVolumeFx();
	//sldMusicVolume->bounds.x = sldMusicVolume->sliderBounds.x + (sldMusicVolume->value * ((float)(sldMusicVolume->sliderBounds.w - sldMusicVolume->bounds.w) / 128.0f));
	//sldFxVolume->bounds.x = sldFxVolume->sliderBounds.x + (sldFxVolume->value * ((float)(sldFxVolume->sliderBounds.w - sldFxVolume->bounds.w) / 128.0f));


	cbFullscreen = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 8, "Fullscreen", { 510, 350, 40, 40 }, this);
	cbVSync = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 9, "VSync", { 900, 350, 40, 40 }, this);

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


	//btnSettings->bounds.x = 550;
	//btnSettings->bounds.y = 275;

	//btnBack2Title->bounds.x = 550;
	//btnBack2Title->bounds.y = 375;

	//btnQuit->bounds.x = 550;
	//btnQuit->bounds.y = 475;

	//btnBack->bounds.x = 550;
	//btnBack->bounds.y = 475;

	if(menuOn)
	{
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		cameraPos = { 0,0 };
	}

	if (settingsOn)
	{

		//sldMusicVolume->bounds.x = 240;
		//sldMusicVolume->bounds.y = 200;
		//sldMusicVolume->sliderBounds.x = 240;
		//sldMusicVolume->sliderBounds.y = 200;

		//sldFxVolume->bounds.x = cameraPos.x + 660;
		//sldFxVolume->bounds.y = cameraPos.y + 200;
		//sldFxVolume->sliderBounds.x = cameraPos.x + 660;
		//sldFxVolume->sliderBounds.y = cameraPos.y + 200;
	}

	//cbFullscreen->bounds.x = 510;
	//cbFullscreen->bounds.y = 350;

	cbVSync->bounds.x = 900;
	cbVSync->bounds.y = 350;

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
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && player->heDed == false) { app->SaveRequest(); }
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && player->heDed == false) { app->LoadRequest(); }

	app->map->Draw();

	btnResume->Update(dt);
	btnSettings->Update(dt);
	btnBack2Title->Update(dt);
	btnQuit->Update(dt);
	btnBack->Update(dt);

	app->titleScreen->sldMusicVolume->Update(dt);
	app->titleScreen->sldFxVolume->Update(dt);
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

	if (player->heDed == true) { 
		app->render->DrawTexture(deathScreenTexture, cameraPos.x + 200,cameraPos.y + 250, nullptr);
	}
	if (menuOn || settingsOn)app->render->DrawTexture(menuBackgroundTexture, 0, 0, false);
	if (menuOn && !settingsOn)
	{
		app->entityManager->doLogic = false;
		btnResume->Draw();
		btnSettings->Draw();
		btnBack2Title->Draw();
		btnQuit->Draw();
	}
	if (settingsOn)
	{
		
		app->titleScreen->sldMusicVolume->Draw();
		app->titleScreen->sldFxVolume->Draw();
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
	app->entityManager->Disable();
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
		app->titleScreen->sldMusicVolume->state = GuiControlState::NORMAL;
		app->titleScreen->sldFxVolume->state = GuiControlState::NORMAL;
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
		app->entityManager->doLogic = true;
		menuOn = false;
	} break;
	case 11://Title Screen
	{
		app->entityManager->Disable();
		app->collisions->Disable();
		app->scene->Disable();
		
		app->transition->TransitionStep(this, (Module*)app->titleScreen, false, 10.0f);

	} break;
	case 12://Back
	{
		settingsOn = false;
		app->titleScreen->sldMusicVolume->state = GuiControlState::DISABLED;
		app->titleScreen->sldFxVolume->state = GuiControlState::DISABLED;
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