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

	// Animation
	/*for (int i = 0; i != 6; ++i)
	{
		rotating.PushBack({ (i * 64) + 384,0, 64, 64 });
	}
	rotating.speed = 15.0f;
	rotating.loop = true;

	currentAnim = &rotating;

	rotating.Reset();*/
}

bool Coin::Update(float dt)
{
	
	//currentAnim->Update();
	return true;
}

bool Coin::Draw()
{
	app->render->DrawTexture(app->entityManager->coinTexture, entityRect.x, entityRect.y, false, invert);

	if (app->render->drawLayerColliders)
	{
		app->render->DrawRectangle({ entityRect.x, entityRect.y,64, 64 }, 255, 255, 0, 100);
	}
	return true;
}

void Coin::OnCollision(Collider* c1, Collider* c2)
{
	/*app->scene->coinCounter++;
	app->scene->scoreValue += 50;*/
	app->audio->PlayFx(app->entityManager->coinSFX);
	this->pendingToDelete = true;
	this->collider->pendingToDelete = true;
}