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

	
	int x, y = 0;
	int w, h = 64;
	int speedX, speedY = 0;   // for a step when pressing the key
	int jumps = 2;
	bool heDed = false; //Checks if player is dead


	SDL_Rect playerRect = { x , y , w, h };

private:

	enum CollisionType {
		Solid,
		SmolSolid,
		Spike,
		Platform,
		Box,
		Spawnpoint,
		End
	};

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

	int GetColliderId(int x, int y) const;

	Collider* playerCollider = nullptr;
	bool positiveSpeedX = true;
	bool positiveSpeedY = true;
	CollisionType GetCollisionType(int A, int B) const;


	iPoint spawnPoint;

	Physics playerPhysics;

	fPoint physicsSpeed; //for when updating Physics, the actual speed it has

	//Pointer to current player animation
	Animation* currentAnimation = &idle;
};

#endif