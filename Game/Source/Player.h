#ifndef __PLAYER_H
#define __PLAYER_H

#include "Module.h"
#include "Animation.h"
#include "Physics.h"
#include "Collisions.h"

#include "SDL/include/SDL.h"


class Player : public Module {
public:
	//Constructor
	Player();

	//Destructor
	~Player();

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

	void Init();

	int jumps;
	bool heDed = false; //Checks if player is dead


	int spawnpointX = 1600;
	int spawnpointY = 5120;

	int checkpointX = spawnpointX;
	int checkpointY = spawnpointY;

	SDL_Rect playerRect = { 0 , 0 , 64, 64 };
	SDL_Rect specialBarRectOne = { 0 , 0 , 64, 15 };
	SDL_Rect specialBarRectTwo = { 0 ,15 , 64, 9 };
	SDL_Rect specialBarRectThree = { 0 ,0 , 1, 1 };
	SDL_Rect specialAttackRect = { 0,0,64,64 };

	enum CollisionType {
		SOLID_SOLID,
		SOLID_AIR,
		AIR_SOLID,
		AIR_AIR,
		//SmolSolid,
		//Spike,
		////Platform,
		//BOX_BOX,
		//BOX_AIR,
		//AIR_BOX,
		//BOX_SOLID,
		//SOLID_BOX,
		//Spawnpoint,
		//End
	};

private:
	//Player and special bar sprites
	SDL_Texture* playerTexture = nullptr;
	SDL_Texture* specialBarTexture = nullptr;

	//Player animations
	Animation idle;
	Animation moving;
	Animation jumping;
	Animation doubleJumping;
	Animation ded;
	Animation jumpDown;
	Animation attack;

	//Special attack animations
	Animation normal;
	Animation breaking;

	int corrector = 0; //Correts attack animation position when it's inverted;
	int specialCorrector = 0;//Correts special animation position when it's inverted;
	int barCounter = 0; //Used for better special bar chargeup

	bool charged = false;//Determines whether you can use your special attack or not

	bool inverted = false; //Checks if animation is inverted

	bool specialInverted = false; //Checks if the special attack goes left or right

	bool godLike;  //God Mode Debug Option

	//int GetTileProp(int x, int y, const char* property) const;

	//void resolveCollisions(iPoint nextFrame, bool positiveSpeedY);

	Collider* playerCollider = nullptr;
	bool positiveSpeedX = true;
	bool positiveSpeedY = true;
	
	bool boxcorrectedonce = false;

	

	iPoint spawnPoint;
	Physics playerPhysics;
	iPoint nextFrame;

	//Pointer to current player animation
	Animation* currentAnimation = &idle;
	Animation* currentSpecialAttackAnimation = &normal;

	//Player SFX
	unsigned int jumpSFX;
	unsigned int doubleJumpSFX;
	unsigned int deathSFX;
	unsigned int coinSFX;
	unsigned int attackSFX;
	unsigned int specialSFX;
	bool alreadyPlayed = false;
};

#endif