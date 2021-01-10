#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

struct SDL_Texture;

class Scene : public Module
{
public:
	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void Init();

	bool OnGuiMouseClickEvent(GuiControl* control);

	Entity* player;
	Entity* fly;
	Entity* slime;

	float seconds = 0;
	float minutes = 0;
	int coins = 0;
	int score = 0;

	iPoint cameraPos = { 0,0 };

private:
	SDL_Texture* deathScreenTexture;
	SDL_Texture* menuBackgroundTexture;

	bool respawn = true;

	bool menuOn = false;
	bool settingsOn = false;
	bool exit = false;

	GuiButton* btnResume;
	GuiButton* btnSettings;
	GuiButton* btnBack2Title;
	GuiButton* btnQuit;
	GuiButton* btnBack;

	GuiCheckBox* cbFullscreen;
	GuiCheckBox* cbVSync;

	char timer[8] = { "\0" };
	char coinText[4] = { "\0" };
	char scoreText[12] = { "\0" };

	Entity* coin;
};

#endif // __SCENE_H__