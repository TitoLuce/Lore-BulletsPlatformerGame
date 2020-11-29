#ifndef __ENEMY_H
#define __ENEMY_H

#include "Module.h"
#include "Animation.h"
#include "Physics.h"
#include "Collisions.h"

#include "SDL/include/SDL.h"


class Enemy : public Module {
public:
	//Constructor
	Enemy();

	//Destructor
	~Enemy();

	bool Awake(pugi::xml_node&);

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start();


	bool PreUpdate();
	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt);

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

	bool CleanUp();

	SDL_Rect slimeRect = { 0 , 0 , 64, 64 };

	enum EnemyType {
		SLIME,
		FLY,
	};

private:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* slimeTexture = nullptr;
	SDL_Texture* flyTexture = nullptr;

	//Slime animations
	Animation slimeMoving;
	Animation slimeDed;
	Animation slimeIdle;

	bool slimeInverted = false; //Checks if animation is inverted

	//Enemy states
	bool slimeIsDed = false;

	//Pointer to current enemy animations
	Animation* currentSlimeAnimation = &slimeIdle;
};

#endif