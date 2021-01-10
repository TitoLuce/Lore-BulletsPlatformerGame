#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Point.h"
#include "Animation.h"
#include "Physics.h"
#include "DynArray.h"

#include "Entity.h"

struct SDL_Texture;
class Collider;
enum EnemyType;

class Enemy : public Entity
{
public:
	// Constructor
	// Saves the spawn position for later movement calculations
	Enemy(int x, int y, EnemyType type, Entity* playerPointer);

	// Destructor
	virtual ~Enemy();

	// Called from inhering enemies' Udpate
	// Updates animation and collider position
	virtual bool Update(float dt);

	// Called from ModuleEnemies' Update
	virtual bool Draw();

	// Collision response
	// Triggers an animation and a sound fx

	void OnCollision(Collider* c1, Collider* c2);

public:


	// Add to despawn queue



	int pathCount = 0;
	// The enemy's path
	DynArray<iPoint> path;


	// Sound fx when destroyed
	int chasingFx = 0;
	int destroyedFx = 0;


	//Player SFX

	bool alreadyPlayed = false;



protected:
	// A ptr to the current animation
	/*Animation* currentAnim = nullptr;
	bool invert = false;*/
	Entity* player;

	int enemySize = 64;


	// Original spawn position. Stored for movement calculations
	iPoint spawnPos;

	iPoint pastDest;
	int i;
	int counterTile;
	DynArray<iPoint> currentPath;

	// State changes

	bool hurtChange = false;
};

#endif // __ENEMY_H__