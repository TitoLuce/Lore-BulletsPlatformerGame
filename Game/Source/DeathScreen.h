//#ifndef __DEATH_SCREEN_H
//#define __DEATH_SCREEN_H
//
//#include "Module.h"
//#include "SDL/include/SDL.h"
//
//class TitleScreen : public Module {
//public:
//	//Constructor
//	DeathScreen();
//
//	//Destructor
//	~DeathScreen();
//
//	// Called when the module is activated
//	// Loads the necessary textures for the map background
//	bool Start() override;
//
//	bool PreUpdate();
//	// Called at the middle of the application loop
//	// Updates the scene's background animations
//	bool Update(float dt);
//
//	// Called at the end of the application loop.
//	// Performs the render call of all the parts of the scene's background
//	bool PostUpdate();
//
//	bool CleanUp();
//
//	void Init();
//
//private:
//	// The scene sprite sheet loaded into an SDL_Texture
//	SDL_Texture* deathScreenTexture;
//};
//
//#endif