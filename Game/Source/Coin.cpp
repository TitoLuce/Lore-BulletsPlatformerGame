#include "Coin.h"

#include "App.h"
#include "Audio.h"
#include "Render.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Animation.h"

Coin::Coin(int x, int y) : Entity(x, y, EntityType::COIN)
{
	pendingToDelete = false;
	entityRect = { x, y, 64, 64 };
	collider = app->collisions->AddCollider(entityRect, Collider::Type::COIN, (Module*)app->entityManager);

	invert = false;

	physics.axisX = false;
	physics.axisY = false;
	physics.positiveSpeedY = false;
	physics.verlet = false;

	//Animation
	for (int i = 0; i != 6; ++i) { rotating.PushBack({ (i * 64),0, 64, 64 }); }
	rotating.SetSpeed(0.15f);

	rotating.Reset();
	currentAnim = &rotating;
}

bool Coin::Update(float dt)
{

	return true;
}

bool Coin::Draw()
{
	currentAnim->Update();
	app->render->DrawTexture(app->entityManager->coinTexture, entityRect.x, entityRect.y, &currentAnim->GetCurrentFrame());

	if (app->render->drawLayerColliders)
	{
		app->render->DrawRectangle({ entityRect.x, entityRect.y,64, 64 }, 255, 255, 0, 100);
	}
	return true;
}

void Coin::OnCollision(Collider* c1, Collider* c2)
{
	app->scene->coins++;
	app->scene->score += 50;
	app->audio->PlayFx(app->entityManager->coinSFX);
	this->pendingToDelete = true;
	this->collider->pendingToDelete = true;
}