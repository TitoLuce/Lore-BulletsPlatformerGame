#include "EnemySlime.h"

#include "App.h"
#include "Collisions.h"
#include "PathFinding.h"
#include "Player.h"
#include "Audio.h"
#include "Render.h"
#include "Enemy.h"

#include "Log.h"

EnemySlime::EnemySlime(int x, int y, EnemyType typeOfEnemy) : Enemy(x, y, typeOfEnemy)
{
	//Slime Animations
	for (int i = 0; i < 5; i++)
	{
		slimeIdle.PushBack({ i * 64,128,64,48 });
	}
	slimeIdle.SetSpeed(0.14f);

	for (int i = 0; i < 7; i++)
	{
		slimeMoving.PushBack({ i * 64,0,64,48 });
	}
	slimeMoving.SetSpeed(0.14f);


	for (int i = 0; i < 7; i++)
	{
		slimeDed.PushBack({ i * 64,60,64,48 });
	}
	slimeDed.SetSpeed(0.14f);

	slimeDed.SetLoop(false);
	invert = true;

	currentAnim = &slimeIdle;



	collider = app->collisions->AddCollider({ enemyRect.x, enemyRect.y, 64, 64 }, Collider::Type::ENEMY, (Module*)app->enemies);

}

void EnemySlime::Update(float dt)
{
	nextFrame.x = enemyRect.x;
	nextFrame.y = enemyRect.y;
	enemyPhysics.speed.x = 0;
	enemyPhysics.CheckDirection();


	if (enemyPhysics.speed.x != 0)
	{
		currentAnim = &slimeMoving;
	}

	if (currentAnim->HasFinished() && enemyPhysics.speed.x == 0)
	{
		if (currentAnim == &slimeDed)
		{
			pendingToDelete = true;
		}
		else
		{
			 if (currentAnim == &slimeMoving)
			{
				 slimeMoving.Reset();
			}
			currentAnim = &slimeIdle;
		}
	}

	if (app->map->GetTileProperty(nextFrame.x / 64, nextFrame.y / 64 + 1, "Collider") == Collider::Type::PAIN)
	{
		hurtChange = true;
		collider->pendingToDelete = true;
		app->audio->PlayFx(deathSFX, 40,0);
	}

	iPoint origin = { nextFrame.x /64,nextFrame.y /64 };
	iPoint destination = { app->player->playerRect.x / 64,app->player->playerRect.y / 64 };
	if (destination.y < 0)
	{
		destination.y = 0;
	}
	if (pastDest != destination)
	{
		pastDest = destination;
		if (origin.x != destination.x || origin.y != destination.y)
		{
			path.Clear();
			pathCount = app->pathfinding->CreatePath(path, origin, destination);
			if (pathCount != -1)
			{
				//LOG("origin: %d, %d destination: %d, %d\n", origin.x, origin.y, destination.x, destination.y);
				i = 0;
			}
		}
	}

	if (pathCount < 12 && pathCount > 1 && !hurtChange)
	{
		if (i >= (pathCount - 2))
		{
			i = pathCount - 2;
		}

		iPoint pos = app->map->MapToWorld(path.At(i)->x, path.At(i)->y);
		iPoint dest = app->map->MapToWorld(path.At(i+1)->x, path.At(i+1)->y);
		iPoint dif = { dest.x - pos.x,dest.y - pos.y };
		//LOG("dif: %d, %d\n", dif.x, dif.y);
		if (dif.x > 0)
		{
			
			currentAnim = &slimeMoving;
			enemyPhysics.speed.x = 150.0f;
			invert = false;
		}
		else if (dif.x < 0)
		{
			currentAnim = &slimeMoving;
			origin.x = (nextFrame.x + enemyRect.w) / 64;
			enemyPhysics.speed.x = -75.0f;
			invert = true;
		}

		if (dif.y < 0)
		{
			if (enemyPhysics.speed.y == 0)
			{
				enemyPhysics.speed.y = -250.0f;
			}
			enemyPhysics.positiveSpeedY = false;
		}
		else if (dif.y > 0)
		{
			enemyPhysics.positiveSpeedY = true;
		}

		counterTile++;
		if (counterTile >= 32)
		{
			counterTile = 32;
		}
		if (counterTile == 64 / 2 && enemyPhysics.speed.y == 0)
		{
			i++;
			counterTile = 0;
		}
	}

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update(dt);
}