#ifndef LOGO_SCREEN_H
#define LOGO_SCREEN_H

#include "Module.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

class LogoScreen : public Module {
public:
	//Constructor
	LogoScreen();

	//Destructor
	~LogoScreen();

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

private:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* logoTitleTexture = nullptr;
	SDL_Texture* gameTitle;
	int timer = 0;
};

#endif