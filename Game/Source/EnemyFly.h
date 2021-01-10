#ifndef __ENEMY_FLYING_H__
#define __ENEMY_FLYING_H__

#include "Enemy.h"

class EnemyFly : public Enemy
{
public:
	// Constructor (x y coordinates in the world)
	// Creates animation and movement data and the collider
	EnemyFly(int x, int y, EnemyType typeOfEnemy, Entity* playerPointer);

	// The enemy is going to follow the different steps in the path
	// Position will be updated depending on the speed defined at each step
	bool Update(float dt);

private:
	// This enemy has one sprite and one frame
	// We are keeping it an animation for consistency with other enemies

	//Fly animations
	Animation flyDed;
	Animation flyIdleOrMoving;
	//Animation* currentFlyAnimation = &flyIdleOrMoving;
	bool flyInverted = false;
};

#endif // __ENEMY_GROUND_H__