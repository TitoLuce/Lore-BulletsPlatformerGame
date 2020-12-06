#include "EnemyFly.h"

#include "App.h"
#include "Collisions.h"
#include "PathFinding.h"
#include "Player.h"
#include "Audio.h"
#include "Render.h"
#include "Enemy.h"

#include "Log.h"

EnemyFly::EnemyFly(int x, int y, EnemyType typeOfEnemy) : Enemy(x, y, typeOfEnemy)
{

	////Fly animations
	//for (int i = 0; i < 3; i++)
	//{
	//	flyIdleOrMoving.PushBack({ i * 62,70,62,54 });
	//}
	//flyIdleOrMoving.SetSpeed(0.08f);

	//for (int i = 0; i < 5; i++)
	//{
	//	flyDed.PushBack({ i * 62,0,62,54 });
	//}
	//flyDed.PushBack({ 0,0,1,1 });
	//flyDed.SetSpeed(0.14f);
	//flyDed.SetLoop(false);
}

void EnemyFly::Update(float dt)
{
	nextFrame.x = enemyRect.x;
	nextFrame.y = enemyRect.y;
	enemyPhysics.speed.x = 0;
	enemyPhysics.speed.y = 0;
	enemyPhysics.CheckDirection();

	if (hurtChange)
	{
		currentAnim = &flyDed;
	}

	if (currentAnim->HasFinished())
	{
		if (currentAnim == &flyDed)
		{
			pendingToDelete = true;
		}
	}

	if (app->map->GetTileProperty(nextFrame.x / 64, nextFrame.y / 64 + 1, "Collider") == Collider::Type::PAIN)
	{
		hurtChange = true;
		collider->pendingToDelete = true;
		app->audio->PlayFx(deathSFX, 40,0);
	}

	iPoint origin = { nextFrame.x / 64,nextFrame.y / 64 };
	iPoint destination = { app->player->playerRect.x / 64,app->player->playerRect.y /64 };
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
		iPoint dest = app->map->MapToWorld(path.At(i + 1)->x, path.At(i + 1)->y);
		iPoint dif = { dest.x - pos.x,dest.y - pos.y };
		//LOG("dif: %d, %d\n", dif.x, dif.y);
		if (dif.x > 0)
		{
			enemyPhysics.speed.x = 150.0f;
			invert = false;
		}
		else if (dif.x < 0)
		{
			origin.x = (nextFrame.x + enemyRect.w) / 64;
			enemyPhysics.speed.x = -75.0f;
			invert = true;
		}
		else if (dif.y < 0)
		{
			origin.y = (nextFrame.y + enemyRect.h) / 64;
			enemyPhysics.speed.y = -75.0f;
		}
		else if (dif.y > 0)
		{
			enemyPhysics.speed.y = 150.0f;
		}
	}

	counterTile++;
	if (counterTile == 64/2)
	{
		i++;
		counterTile = 0;
	}

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update(dt);
}