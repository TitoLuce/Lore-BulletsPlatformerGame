#include "EnemyFly.h"

#include "App.h"
#include "Collisions.h"
#include "PathFinding.h"
#include "Player.h"
#include "Audio.h"
#include "Render.h"
#include "Enemy.h"
#include "EntityManager.h"
#

#include "Log.h"

EnemyFly::EnemyFly(int x, int y, EnemyType typeOfEnemy, Entity* playerPointer) : Enemy(x, y, typeOfEnemy, playerPointer)
{

	entityRect = { x, y, 64, 64 };

	collider = app->collisions->AddCollider(entityRect, Collider::Type::ENEMY, (Module*)app->entityManager);


	//Fly animations
	for (int i = 0; i < 3; i++)
	{
		flyIdleOrMoving.PushBack({ i * 62,70,62,54 });
	}
	flyIdleOrMoving.SetSpeed(0.08f);

	for (int i = 0; i < 5; i++)
	{
		flyDed.PushBack({ i * 62,0,62,54 });
	}
	flyDed.PushBack({ 0,0,1,1 });
	flyDed.SetSpeed(0.14f);
	flyDed.SetLoop(false);


	currentAnim = &flyIdleOrMoving;



}

bool EnemyFly::Update(float dt)
{
	nextPos.x = entityRect.x;
	nextPos.y = entityRect.y;
	physics.speed.x = 0;
	physics.speed.y = 0;
	physics.CheckDirection();

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



	if (app->map->GetTileProperty(nextPos.x / 64, nextPos.y / 64 + 1, "Collider") == Collider::Type::PAIN)
	{
		hurtChange = true;
		collider->pendingToDelete = true;
		app->audio->PlayFx(app->entityManager->deathSFX, 40, 0);
	}

	iPoint origin = { nextPos.x / 64,nextPos.y / 64 };
	iPoint destination = { player->entityRect.x / 64,player->entityRect.y / 64 };
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
			physics.speed.x = 180.0f;
			invert = false;
		}
		else if (dif.x < 0)
		{
			origin.x = (nextPos.x + entityRect.w) / 64;
			physics.speed.x = -65.0f;
			invert = true;
		}
		else if (dif.y < 0)
		{
			origin.y = (nextPos.y + entityRect.h) / 64;
			physics.speed.y = -65.0f;
		}
		else if (dif.y > 0)
		{
			physics.speed.y = 180.0f;
		}
	}

	counterTile++;
	if (counterTile == 64 / 2)
	{
		i++;
		counterTile = 0;
	}

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update(dt);


	return true;
}