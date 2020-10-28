#ifndef __PLAYER_H
#define __PLAYER_H

#include "Module.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

class Player : public Module {
public:
	//Constructor
	Player();

	//Destructor
	~Player();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt);

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

	bool CleanUp();

	void Init();

	bool isAlive;
	
	int x, y = 0;
	int w, h = 64;
	int speedX, speedY = 0;


private:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* playerTexture = nullptr;

	//Player animations
	Animation idle;
	Animation moving;
	Animation jumping;
	Animation doubleJumping;
	Animation ded;
	Animation jumpDown;
	bool heDed = false;
};

#endif