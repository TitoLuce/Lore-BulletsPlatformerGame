#ifndef __MODULE_TITLE_SCREEN_H
#define __MODULE_TITLE_SCREEN_H

#include "Module.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

class TitleScreen : public Module {
public:
	//Constructor
	TitleScreen();

	//Destructor
	~TitleScreen();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update();

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

	bool CleanUp();

private:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* backgroundTexture = nullptr;
	Animation* titleScreenAnimation;
	SDL_Texture* gameTitle;
};

#endif