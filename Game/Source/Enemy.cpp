#include "Enemy.h"
#include "EnemySlime.h"

#include "App.h"
#include "EntityManager.h"
#include "Collisions.h"
#include "Audio.h"
#include "Render.h"
#include "PathFinding.h"



#include "Log.h"

Enemy::Enemy(int x, int y, EnemyType eType, Entity* playerPointer) : Entity(x, y, EntityType::ENEMY, eType)
{
	player = playerPointer;
	spawnPos.x = x;
	spawnPos.y = y;
	//destroyedFx = app->entityManager->enemyDestroyedFx;  our sound effect
	pendingToDelete = false;
	physics.axisX = true;
	physics.axisY = true;
	path.Create(DEFAULT_PATH_LENGTH);
}

Enemy::~Enemy()
{
	if (collider != nullptr)
		collider->pendingToDelete = true;
	path.Clear();
}

bool Enemy::Update(float dt)
{
	ListItem<Entity*>* e = app->entityManager->entities.start;
	while (e != nullptr)
	{
		if (e->data->type == EntityType::PLAYER)
		{
			player = e->data;
		}
		e = e->next;
	}

	if (currentAnim != nullptr)
		currentAnim->Update();

	physics.UpdatePhysics(nextPos, dt);
	physics.ResolveCollisions(entityRect, nextPos, invert);

	if (collider != nullptr)
		collider->SetPos(entityRect.x, entityRect.y, currentAnim->GetCurrentFrame().w, currentAnim->GetCurrentFrame().h);

	return true;
}

bool Enemy::Draw()
{
	if (currentAnim != nullptr)
	{
		if (eType == GROUND)
			app->render->DrawTexture(app->entityManager->slimeTexture, entityRect.x, entityRect.y, &(currentAnim->GetCurrentFrame()));
		else if (eType == FLYING) {
			app->render->DrawTexture(app->entityManager->flyTexture, entityRect.x, entityRect.y, &(currentAnim->GetCurrentFrame()));
		}
	}

	if (app->render->drawLayerColliders)
	{
		app->render->DrawRectangle({ entityRect.x, entityRect.y, 64,64 }, 255, 255, 0, 100);

		app->pathfinding->DrawPath(&path);
	}
	return true;
}

void Enemy::OnCollision(Collider* c1, Collider* c2)
{

	if (c2->type == Collider::Type::ATTACK)
	{

		hurtChange = true;
		collider->pendingToDelete = true;
		app->audio->PlayFx(app->entityManager->deathSFX, 0);
	}

}