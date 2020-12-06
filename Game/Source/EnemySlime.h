#ifndef __ENEMY_GROUND_H__
#define __ENEMY_GROUND_H__

#include "Enemy.h"

class EnemySlime : public Enemy
{
public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	EnemySlime(int x, int y, EnemyType typeOfEnemy);

	// The enemy is going to follow the different steps in the path
	void Update(float dt);

	EnemyType type;
	bool slimeInverted = false;

private:
	//Slime animations
	Animation slimeMoving;
	Animation slimeDed;
	Animation slimeIdle;
	Animation* currentSlimeAnimation = &slimeIdle;

};

#endif // __ENEMY_GROUND_H__