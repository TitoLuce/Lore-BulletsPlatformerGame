#ifndef __TITLE_SCREEN_H
#define __TITLE_SCREEN_H

#include "Module.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

class TitleScreen : public Module
{
public:
	//Constructor
	TitleScreen();

	//Destructor
	~TitleScreen();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	bool PreUpdate();
	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt);

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

	bool CleanUp();

	void Init();

	bool OnGuiMouseClickEvent(GuiControl* control);

private:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* backgroundTexture;
	SDL_Texture* gameTitle;
	SDL_Texture* menuBackgroundTexture;

	bool settingsOn = false;
	bool creditsOn = false;
	bool exit = false;

	GuiButton* btnStart;
	GuiButton* btnContinue;
	GuiButton* btnSettings;
	GuiButton* btnCredits;
	GuiButton* btnQuit;

	GuiSlider* sldMusicVolume;
	GuiSlider* sldFxVolume;

	GuiCheckBox* cbFullscreen;
	GuiCheckBox* cbVSync;

public:

	int font;
	int font2;
};

#endif