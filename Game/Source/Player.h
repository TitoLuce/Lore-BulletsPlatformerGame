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

	int jumps = 2;
	bool heDed = false; //Checks if player is dead


	int spawnpointX = 1600;
	int spawnpointY = 5120;

	int checkpointX = spawnpointX;
	int checkpointY = spawnpointY;

	SDL_Rect playerRect = { 0 , 0 , 64, 64 };


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
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* playerTexture = nullptr;

	//Player animations
	Animation idle;
	Animation moving;
	Animation jumping;
	Animation doubleJumping;
	Animation ded;
	Animation jumpDown;

	bool inverted = false; //Checks if animation is inverted
	

	bool godLike;  //God Mode Debug Option

	int GetTileProp(int x, int y, const char* property) const;

	Collider* playerCollider = nullptr;
	bool positiveSpeedX = true;
	bool positiveSpeedY = true;
	CollisionType GetCollisionType(int A, int B) const;

	iPoint spawnPoint;
	Physics playerPhysics;
	fPoint physicsSpeed; 

	//Pointer to current player animation
	Animation* currentAnimation = &idle;

	//Player SFX
	unsigned int jumpSFX;
	unsigned int doubleJumpSFX;
	unsigned int deathSFX;
	unsigned int coinSFX;
	bool alreadyPlayed = false;
};

#endif