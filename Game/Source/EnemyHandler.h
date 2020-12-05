#ifndef __ENEMY_HANDLER_H
#define __ENEMY_HANDLER_H

#include "Module.h"
#include "Animation.h"
#include "Physics.h"
#include "Collisions.h"

#include "SDL/include/SDL.h"


class EnemyHandler : public Module {
public:
	//Constructor
	EnemyHandler();

	//Destructor
	~EnemyHandler();

	void Init();


	bool Awake(pugi::xml_node& config);

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start();


	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt);

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

	bool CleanUp();

	SDL_Rect slimeRect = { 0 , 0 , 64, 64 };
	SDL_Rect flyRect = { 0 , 0 , 64, 64 };
	SDL_Rect goblinRect = { 0 , 0 , 64, 64 };

	enum EnemyType {
		SLIME,
		FLY,
		GOBLIN,
	};

private:
	SDL_Texture* slimeTexture = nullptr;
	SDL_Texture* flyTexture = nullptr;
	SDL_Texture* goblinTexture = nullptr;

	//Slime animations
	Animation slimeMoving;
	Animation slimeDed;
	Animation slimeIdle;

	//Fly animations
	Animation flyDed;
	Animation flyIdleOrMoving;

	//Goblin animations
	Animation goblinMoving;
	Animation goblinDed;
	Animation goblinIdle;
	Animation goblinHit;

	//Checks if animation is inverted
	bool slimeInverted = false;
	bool flyInverted = false;
	bool goblinInverted = false;

	//Enemy states
	bool slimeIsDed = false;
	bool flyIsDed = false;
	bool goblinIsDed = false;
	int goblinHP = 2;
	bool goblinIsHit = false;
	int goblinCounter = 0;

	//Pointer to current enemy animations
	Animation* currentSlimeAnimation = &slimeIdle;
	Animation* currentFlyAnimation = &slimeIdle;
	Animation* currentGoblinAnimation = &slimeIdle;

	//Player SFX
	unsigned int deathSFX;
	unsigned int hitSFX;
	bool alreadyPlayed = false;
};
#endif