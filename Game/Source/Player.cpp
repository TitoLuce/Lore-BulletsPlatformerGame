#include "Player.h"

#include "App.h"
#include "Window.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "Transition.h"
#include "Scene.h"
#include "Map.h"
#include "Defs.h"
#include "Log.h"

#include "Input.h"

#include "SDL/include/SDL_scancode.h"

Player::Player()
{
	name.Create("player");
}

Player::~Player() {}

// Load assets
bool Player::Start()
{
	app->transition->TransitionStep(nullptr, this, true, 1200.0f);
	playerTexture = app->tex->Load("Assets/PlayerSprites.png");
	specialBarTexture = app->tex->Load("Assets/SpecialBar.png");
	jumpSFX = app->audio->LoadFx("Assets/Audio/fx/JumpOne.wav");
	doubleJumpSFX = app->audio->LoadFx("Assets/Audio/fx/JumpTwo.wav");
	deathSFX = app->audio->LoadFx("Assets/Audio/fx/Death.wav");
	coinSFX = app->audio->LoadFx("Assets/Audio/fx/Coin.wav");
	attackSFX = app->audio->LoadFx("Assets/Audio/fx/Attack.wav");
	specialSFX = app->audio->LoadFx("Assets/Audio/fx/Special.wav");


	playerRect = { spawnpointX,spawnpointY,idle.GetCurrentFrame().w,idle.GetCurrentFrame().h };
	specialAttackRect = { 0,0,normal.GetCurrentFrame().w,normal.GetCurrentFrame().h };

	specialBarRectOne = { 0 , 0 , 64, 15 };
	specialBarRectTwo = { 0 ,15 , 64, 9 };
	specialBarRectThree = { playerRect.x + 5,playerRect.y + 73,0,9 };

	//Reset animations
	moving.Reset();
	ded.Reset();

	playerPhysics.axisY = true;
	playerPhysics.axisX = true;
	playerPhysics.positiveSpeedY = true;
	isPlatform = false;

	jumps = 2;
	heDed = false;
	inverted = false;
	specialInverted = false;
	godLike = false;
	corrector = 0;
	specialCorrector = false;
	charged = false;
	barCounter = 0;

	return true;
}

bool Player::Awake(pugi::xml_node&)
{
	for (int i = 0; i < 4; i++)
	{
		idle.PushBack({ 64*i,256,64,64 });
	}
	idle.SetSpeed(0.14f);

	for (int i = 0; i < 4; i++)
	{
		moving.PushBack({ 64 * i,64,64,64 });
	}
	moving.SetSpeed(0.14f);
	
	for (int i = 0; i < 3; i++)
	{
		jumping.PushBack({ 64 * i,384,64,64 });
	}
	jumping.SetSpeed(0.08f);


	for (int i = 0; i < 7; i++)
	{
		ded.PushBack({ 64 * i,0,64,64 });
	}
	ded.PushBack({ 0,0,0,0 });
	ded.SetSpeed(0.15f);
	ded.SetLoop(false);


	for (int i = 0; i < 3; i++)
	{
		jumpDown.PushBack({ 64 * i,320,64,64 });
	}
	jumpDown.SetSpeed(0.08f);

	for (int i = 0; i < 4; i++)
	{
		attack.PushBack({ i * 128,192,128,64 });
	}
	attack.SetSpeed(0.4);
	attack.SetLoop(false);

	for (int i = 0; i < 3; i++)
	{
		doubleJumping.PushBack({ i * 64,512,64,64 });
	}
	doubleJumping.SetSpeed(0.3f);

	for (int i = 0; i < 3; i++)
	{
		normal.PushBack({ i * 64,704,64,64 });
	}
	normal.SetSpeed(0.3f);

	for (int i = 0; i < 5; i++)
	{
		breaking.PushBack({ i * 64,640,64,64 });
	}
	breaking.SetSpeed(0.4f);
	breaking.SetLoop(false);

	return true;
}

bool Player::PreUpdate() { return true; }

bool Player::Update(float dt)
{
	// To know what direction the velocity is going
	playerPhysics.CheckDirection();

	nextFrame.x = playerRect.x;
	nextFrame.y = playerRect.y;

	if (heDed)
	{
		if (!alreadyPlayed)
		{
			app->audio->PlayFx(deathSFX, 50, 0);
			alreadyPlayed = true;
		}
		
		if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			barCounter = 0;
			heDed = false;
			ded.Reset();
			corrector = 0;
			jumps = 2;
			playerRect.x = checkpointX;
			playerRect.y = checkpointY;
			alreadyPlayed = false;
		}
	}
	else
	{
		if (currentAnimation != &attack && currentAnimation != &jumping && currentAnimation != &doubleJumping) { currentAnimation = &idle; }
		//Special bar loading
		if (!charged)
		{
			barCounter++;
			if (barCounter >= 45)
			{
				specialBarRectThree.w += 1;
				barCounter = 0;
			}
			if (specialBarRectThree.w >= 54)
			{
				charged = true;
			}
		}
		//Special attack
		else if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN && currentAnimation != &doubleJumping && currentAnimation != &jumping)
		{
			if (inverted) { corrector = 64; }
			else { corrector = 0; }
			if (inverted)
			{
				specialCorrector = 64;
				specialInverted = true;
			}
			else
			{
				specialCorrector = -64;
				specialInverted = false;
			}

			app->audio->PlayFx(specialSFX, 40, 0);
			specialAttackRect.x = playerRect.x;
			specialAttackRect.y = playerRect.y;
			currentAnimation = &attack;
			currentSpecialAttackAnimation = &normal;
			specialBarRectThree.w = 0;
			charged = false;
		}

		if (!breaking.HasFinished() && currentSpecialAttackAnimation == &normal)
		{
			if (specialInverted) { specialAttackRect.x -= 6; }
			else { specialAttackRect.x += 6; }
		}

		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			if (!godLike) {
				godLike = true;
				playerPhysics.axisY = false;
				playerPhysics.axisX = false;
			}
			else
			{
				godLike = false;
				playerPhysics.axisY = true;
				playerPhysics.axisX = true;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && currentAnimation != &doubleJumping && currentAnimation != &jumping && currentAnimation != &attack)
		{
			if (inverted) { corrector = 64; }
			else { corrector = 0; }

			app->audio->PlayFx(attackSFX, 40, 0);
			currentAnimation = &attack;
		}
		if (attack.HasFinished())
		{
			corrector = 0;
			currentAnimation = &idle;
			attack.Reset();
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && currentAnimation != &attack)
		{
			inverted = false;

			if (currentAnimation != &jumping && currentAnimation != &doubleJumping)
			{
				currentAnimation = &moving;
			}
			if (godLike)
			{
				nextFrame.x += floor(250.0f * dt);
			}
			else
			{
				playerPhysics.speed.x = 250.0f;
				positiveSpeedX = true;
			}

		}
		else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && currentAnimation != &attack)
		{
			inverted = true;

			if (currentAnimation != &jumping && currentAnimation != &doubleJumping)
			{
				currentAnimation = &moving;
			}

			if (godLike)
			{
				nextFrame.x -= floor(250.0f * dt);
			}
			else
			{
				playerPhysics.speed.x = -250.0f;
				positiveSpeedX = false;
			}
		}
	else 
		{
			playerPhysics.speed.x = 0;
		}

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			if (jumps == 2)
			{
				app->audio->PlayFx(jumpSFX, 40, 0);
				playerPhysics.speed.y = -500.0f;
				currentAnimation = &jumping;
			}
			else if (jumps == 1)
			{
				app->audio->PlayFx(doubleJumpSFX, 40, 0);
				playerPhysics.speed.y = -500.0f;
				currentAnimation = &doubleJumping;
			}
			jumps--;
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && godLike == true)
		{
			positiveSpeedY = false;
			nextFrame.y -= floor(250.0f * dt);
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			if (godLike)
			{
				positiveSpeedY = true;
				currentAnimation = &jumpDown;
				nextFrame.y += floor(250.0f * dt);
			}
			if (GetTileProp(playerRect.x / 64, (playerRect.y / 64) + 1, "Collider") == Collider::Type::BOX)
			{
				isPlatform = true;
				currentAnimation = &jumpDown;
				playerPhysics.speed.y = 200.0f;
			}
		}
		//else
		//{
		//	isPlatform = false;
		//}

		if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		{
			currentAnimation = &ded;
			heDed = true;	
		}

		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		{
			nextFrame.x = spawnpointX;
			nextFrame.y = spawnpointY;
		}


		//physics
		playerPhysics.UpdatePhysics(nextFrame, dt);

		//collision 
		
		playerPhysics.ResolveCollisions(playerRect, nextFrame, inverted, isPlatform);

		if (GetTileProp(playerRect.x / 64, playerRect.y / 64 + 1, "Collider") == Collider::Type::SOLID && currentAnimation != &moving && currentAnimation != &attack && !heDed)
		{
			currentAnimation = &idle;
			jumps = 2;
		}

		// Dead
		if ((GetTileProp(playerRect.x / 64, (playerRect.y + 1) / 64, "Collider") == Collider::Type::PAIN || GetTileProp(playerRect.x / 64, (playerRect.y - 1) / 64, "Collider") == Collider::Type::PAIN || GetTileProp((playerRect.x + 1) / 64, playerRect.y / 64, "Collider") == Collider::Type::PAIN || GetTileProp((playerRect.x - 1) / 64, playerRect.y / 64, "Collider") == Collider::Type::PAIN) && !godLike)
		{
			currentAnimation = &ded;
			heDed = true;
		}

		//Coin
		if (GetTileProp(playerRect.x / 64, playerRect.y / 64 + 1, "Collider") == Collider::Type::COIN)
		{
			if (app->map->GetTileProperty(playerRect.x / 64, playerRect.y / 64 + 1, "Drawable", true) == 1)
			{
				specialBarRectThree.w += 3;
				app->map->SetTileProperty(playerRect.x / 64, playerRect.y / 64 + 1, "Drawable", 0, true, true);
			}
			
			
		}
		if (specialBarRectThree.w >= 54)
		{
			specialBarRectThree.w = 54;
		}
	}
	return true;
}

bool Player::PostUpdate()
{
	//Update special bar positions
	specialBarRectThree.x = playerRect.x + 5;
	specialBarRectThree.y = playerRect.y + 73;
	// Draw everything --------------------------------------
	currentAnimation->Update();
	app->render->DrawTexture(playerTexture, playerRect.x - corrector, playerRect.y, &currentAnimation->GetCurrentFrame(), inverted);
	app->render->DrawTexture(specialBarTexture, playerRect.x, playerRect.y + 70, &specialBarRectOne);
	if (!charged) { app->render->DrawRectangle(specialBarRectThree, 0, 191, 255); }
	else { app->render->DrawRectangle(specialBarRectThree, 0, 255, 0); }
	app->render->DrawTexture(specialBarTexture, playerRect.x, playerRect.y + 73, &specialBarRectTwo);
	app->render->DrawTexture(playerTexture, specialAttackRect.x-specialCorrector, specialAttackRect.y, &currentSpecialAttackAnimation->GetCurrentFrame(), specialInverted);
	return true;
}

bool Player::CleanUp()
{
	app->tex->UnLoad(playerTexture);
	return true;
}

void Player::Init()
{
	active = false;
}

int Player::GetTileProp(int x, int y, const char* prop) const
{
	int ret;
	// MapLayer		
	ListItem <MapLayer*>* MapLayerList = app->map->data.layers.start;
	SString layerName;
	layerName = "Collisions";
	while (MapLayerList != NULL)
	{
		if (MapLayerList->data->name == layerName) { break; }
		MapLayerList = MapLayerList->next;
	}

	// Tileset		
	ListItem <TileSet*>* TilesetList = app->map->data.tilesets.start;
	SString tilesetName;

	tilesetName = "Metadata";

	while (TilesetList != NULL) {
		if (TilesetList->data->name == tilesetName) {
			break;
		}
		TilesetList = TilesetList->next;
	}

	// Gets CollisionId
	int id = (MapLayerList->data->Get(x, y) - TilesetList->data->firstgid);	//returns id of the tile
	if (id < 0) 
	{
		ret = 0;
		return ret;
	}
	Tile* currentTile = TilesetList->data->GetPropList(id);
	ret = currentTile->properties.GetProperty(prop, 0);	
	//LOG("%d - %d", id, ret);

	return ret;
}



//
//void Player::resolveCollisions(iPoint nextFrame, bool positiveSpeedY) {
//
//	iPoint tiledPos(playerRect.x / 64, playerRect.y / 64);
//	iPoint correctedPos;
//	iPoint checkedPos;
//	//LOG("past: %d,%d current: %d,%d\n", playerRect.x, playerRect.y, nextFrame.x, nextFrame.y);
//
//	// X axis
//	if (!inverted) { // right
//		tiledPos.x = (playerRect.x + playerRect.w) / 64;
//		int i = 0;
//		while (GetTileProp(tiledPos.x + i, tiledPos.y, "Collider") == Collider::Type::AIR && i < 5) {
//			i++;
//		}
//		i--;
//		correctedPos.x = MIN(nextFrame.x - playerRect.x, (tiledPos.x + i) * 64 - playerRect.x);
//	}
//	else { // left
//		int i = 0;
//		while (GetTileProp(tiledPos.x - i, tiledPos.y, "Collider") == Collider::Type::AIR && i < 5) {
//			i++;
//		}
//		i--;
//		correctedPos.x = -MIN(playerRect.x - nextFrame.x, playerRect.x - (tiledPos.x - i) * 64);
//	}
//
//	// Y axis
//	if (positiveSpeedY) {
//		tiledPos.y = (playerRect.y + playerRect.h) / 64;
//		int i = 0;
//		while (GetTileProp(tiledPos.x, tiledPos.y + i, "Collider") == Collider::Type::AIR && i < 5) {
//			i++;
//		}
//		i--;
//		correctedPos.y = MIN(nextFrame.y - playerRect.y, (tiledPos.y + i) * 64 - playerRect.y);
//	}
//	else {
//		int i = 0;
//		while (GetTileProp(tiledPos.x, tiledPos.y - i, "Collider") == Collider::Type::AIR && i < 5) {
//			i++;
//		}
//		i--;
//		correctedPos.y = -MIN(playerRect.y - nextFrame.y, playerRect.y - (tiledPos.y - i) * 64);
//	}
//
//	playerRect.x += correctedPos.x;
//	playerRect.y += correctedPos.y;
//
//	//
//	//	if (GetTileProp(playerRect.x / 64, playerRect.y / 64 + 1, "Collider") == Collider::Type::SOLID) {
//	//
//	//		physicsSpeed.y = 0.0f;
//	//
//	//	}
//	//
//	//
//	//	else if (GetTileProp(playerRect.x / 64, playerRect.y / 64, "Collider") == Collider::Type::SOLID) {
//	//		physicsSpeed.y = 0.0f;
//	//	}
//	//}
//
//	if (GetTileProp(playerRect.x / 64 + 1, playerRect.y / 64, "CollisionId") == Collider::Type::SOLID
//		&& GetTileProp(playerRect.x / 64, playerRect.y / 64 + 1, "CollisionId") != Collider::Type::SOLID
//		&& !inverted)
//	{
//		playerRect.y -= correctedPos.y;
//		physicsSpeed.x = 0.0f;
//		physicsSpeed.y = 0.0f;
//	}
//	else if (GetTileProp((playerRect.x - 1) / 64, playerRect.y / 64, "CollisionId") == Collider::Type::SOLID
//		&& GetTileProp(playerRect.x / 64, playerRect.y / 64 + 1, "CollisionId") != Collider::Type::SOLID
//		&& inverted)
//	{
//		playerRect.y -= correctedPos.y;
//		physicsSpeed.x = 0.0f;
//		physicsSpeed.y = 0.0f;
//	}
//	else if (GetTileProp(playerRect.x / 64, playerRect.y / 64 + 1, "CollisionId") == Collider::Type::SOLID) {
//		physicsSpeed.y = 0.0f;
//	}
//	else if (GetTileProp(playerRect.x / 64, playerRect.y / 64, "CollisionId") == Collider::Type::SOLID) {
//		physicsSpeed.y = 0.0f;
//	}
//}
