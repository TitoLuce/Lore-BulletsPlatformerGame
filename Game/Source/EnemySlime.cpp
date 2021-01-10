#include "EnemySlime.h"

#include "App.h"
#include "Collisions.h"
#include "PathFinding.h"
#include "Player.h"
#include "Audio.h"
#include "Render.h"
#include "EntityManager.h"
#include "Enemy.h"

#include "Log.h"

EnemySlime::EnemySlime(int x, int y, EnemyType typeOfEnemy, Entity* playerPointer) : Enemy(x, y, typeOfEnemy, playerPointer)
{



	entityRect = { x, y, 64, 64 };

	collider = app->collisions->AddCollider(entityRect, Collider::Type::ENEMY, (Module*)app->entityManager);

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


}

bool EnemySlime::Update(float dt)
{
	nextPos.x = entityRect.x;
	nextPos.y = entityRect.y;
	physics.speed.x = 0;
	physics.CheckDirection();


	if (hurtChange)
	{
		currentAnim = &slimeDed;
	}

	if (physics.speed.x != 0)
	{
		currentAnim = &slimeMoving;
	}

	if (currentAnim->HasFinished() && physics.speed.x == 0)
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

			currentAnim = &slimeMoving;
			physics.speed.x = 150.0f;
			invert = false;
		}
		else if (dif.x < 0)
		{
			currentAnim = &slimeMoving;
			origin.x = (nextPos.x + entityRect.w) / 64;
			physics.speed.x = -75.0f;
			invert = true;
		}

		if (dif.y < 0)
		{
			if (physics.speed.y == 0)
			{
				physics.speed.y = -250.0f;
			}
			physics.positiveSpeedY = false;
		}
		else if (dif.y > 0)
		{
			physics.positiveSpeedY = true;
		}

		counterTile++;
		if (counterTile >= 32)
		{
			counterTile = 32;
		}
		if (counterTile == 64 / 2 && physics.speed.y == 0)
		{
			i++;
			counterTile = 0;
		}
	}

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update(dt);

	return true;
}