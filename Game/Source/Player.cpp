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
	jumpSFX = app->audio->LoadFx("Assets/Audio/fx/JumpOne.wav");
	doubleJumpSFX = app->audio->LoadFx("Assets/Audio/fx/JumpTwo.wav");
	deathSFX = app->audio->LoadFx("Assets/Audio/fx/Death.wav");
	coinSFX = app->audio->LoadFx("Assets/Audio/fx/Coin.wav");

	playerRect = { spawnpointX , spawnpointY , idle.GetCurrentFrame().w, idle.GetCurrentFrame().h };

	//Reset animations
	moving.Reset();
	ded.Reset();

	playerPhysics.axisY = true;
	playerPhysics.axisX = true;

	heDed = false;
	inverted = false;
	godLike = false;
	return true;
}

bool Player::Awake(pugi::xml_node&)
{
	idle.PushBack({ 0,256,64,64 });
	idle.PushBack({ 64,256,64,64 });
	idle.PushBack({ 128,256,64,64 });
	idle.PushBack({ 192,256,64,64 });
	idle.SetSpeed(0.14f);

	moving.PushBack({ 0,64,64,64 });
	moving.PushBack({ 64,64,64,64 });
	moving.PushBack({ 128,64,64,64 });
	moving.PushBack({ 192,64,64,64 });
	moving.PushBack({ 256,64,64,64 });
	moving.PushBack({ 320,64,64,64 });
	moving.SetSpeed(0.14f);

	jumping.PushBack({ 0,384,64,64 });
	jumping.PushBack({ 64,384,64,64 });
	jumping.PushBack({ 128,384,64,64 });
	jumping.SetSpeed(0.08f);

	ded.PushBack({ 0,0,64,64 });
	ded.PushBack({ 64,0,64,64 });
	ded.PushBack({ 128,0,64,64 });
	ded.PushBack({ 192,0,64,64 });
	ded.PushBack({ 256,0,64,64 });
	ded.PushBack({ 320,0,64,64 });
	ded.PushBack({ 384,0,64,64 });
	ded.PushBack({ 0,0,0,0 });
	ded.SetSpeed(0.15f);
	ded.SetLoop(false);

	jumpDown.PushBack({ 0,320,64,64 });
	jumpDown.PushBack({ 64,320,64,64 });
	jumpDown.PushBack({ 128,320,64,64 });
	jumpDown.SetSpeed(0.08f);

	return true;
}

bool Player::PreUpdate() { return true; }

bool Player::Update(float dt)
{
	if (physicsSpeed.y >= 0) positiveSpeedY = true;
	else if (physicsSpeed.y < 0) positiveSpeedY = false;

	//if (physicsSpeed.x >= 0) positiveSpeedX = true;
	//else if (physicsSpeed.x < 0)positiveSpeedX = false;

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
			heDed = false;
			ded.Reset();
			playerRect.x = checkpointX;
			playerRect.y = checkpointY;
			alreadyPlayed = false;
		}
	}
	else
	{
		if (physicsSpeed.y >= 0) { currentAnimation = &idle; }
		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)              //FIX MOVEMENT
		{
			if (!godLike) { godLike = true; }
			else { godLike = false; }
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
		{
			inverted = false;

			if (jumps == 2) { currentAnimation = &moving; }

			nextFrame.x += 5;    
			positiveSpeedX = true;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
		{
			if (!inverted) { inverted = true; }

			if (jumps == 2) { currentAnimation = &moving; }
			nextFrame.x -= 5;
			positiveSpeedX = false;
		}

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			if (jumps >= 2)
			{
				app->audio->PlayFx(jumpSFX, 40, 0);
				physicsSpeed.y = -500;
			}
			else if (jumps >= 1)
			{
				app->audio->PlayFx(doubleJumpSFX, 40, 0);
				physicsSpeed.y = -500;
			}
			jumps--;
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			if (godLike) 
			{
				currentAnimation = &jumpDown;
				physicsSpeed.y = 16.0f;
				nextFrame.y += 5;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && godLike == true)
		{
			physicsSpeed.y = -16.0f;
			nextFrame.y -= 5;
		}

		if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		{
			heDed = true;
			currentAnimation = &ded;
		}

		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		{
			nextFrame.x = spawnpointX;
			nextFrame.y = spawnpointY;
		}



		playerPhysics.UpdatePhysics(nextFrame.x, nextFrame.y, physicsSpeed.x, physicsSpeed.y);

		//collision 
		

		resolveCollisions(nextFrame, positiveSpeedY);

		//if (positiveSpeedX && positiveSpeedY)	//checking the bottom and right sides
		//{
		//	CollisionType collisionType = GetCollisionType(GetTileProp(tilex, tiley + 1, "Collider"), GetTileProp(tilex + 1, tiley, "Collider"));
		//	if (collisionType == CollisionType::SOLID_SOLID)
		//	{
		//		playerRect.y = tiley * 2 * 64 - playerRect.y;
		//		physicsSpeed.y = 0;

		//		playerRect.x = 2 * 64 * (tilex + 1) - 64 * 2 - playerRect.x;
		//		physicsSpeed.x = 0;
		//		jumps = 2;
		//	}
		//	else if (collisionType == CollisionType::AIR_SOLID)
		//	{

		//		physicsSpeed.y -= 5.0f;
		//		if (physicsSpeed.y <= 0) { physicsSpeed.y = 0; }
		//		playerRect.x = 2 * 64 * (tilex + 1) - 64 * 2 - playerRect.x;
		//	}
		//	else if (collisionType == CollisionType::SOLID_AIR)
		//	{
		//		playerRect.y = tiley * 2 * 64 - playerRect.y;
		//		physicsSpeed.y = 0;
		//		physicsSpeed.x = 0;
		//		jumps = 2;
		//	}
		//}
		//else if (positiveSpeedX && !positiveSpeedY)	//checking the top and right corners
		//{
		//	CollisionType collisionType = GetCollisionType(GetTileProp(tilex, tiley, "Collider"), GetTileProp(tilex + 1, tiley + 1, "Collider"));
		//	if (collisionType == CollisionType::SOLID_SOLID)
		//	{
		//		playerRect.y = (tiley + 1) * 2 * 64 - playerRect.y;
		//		physicsSpeed.y = 0;
		//		playerRect.x = 2 * 64 * (tilex)-playerRect.x;
		//	}
		//	else if (collisionType == CollisionType::AIR_SOLID)
		//	{
		//		physicsSpeed.y -= 5.0f;
		//		if (physicsSpeed.y <= 0) { physicsSpeed.y = 0; }
		//		playerRect.x = 2 * 64 * (tilex)-playerRect.x;
		//	}
		//	else if (collisionType == CollisionType::SOLID_AIR)
		//	{
		//		playerRect.y = (tiley + 1) * 2 * 64 - playerRect.y;
		//		physicsSpeed.y = 0;
		//	}
		//}
		//else if (!positiveSpeedX && !positiveSpeedY)	//checking the left and top sides
		//{
		//	CollisionType collisionType = GetCollisionType(GetTileProp(tilex, tiley + 1, "Collider"), GetTileProp(tilex + 1, tiley, "Collider"));
		//	if (collisionType == CollisionType::SOLID_SOLID)
		//	{
		//		playerRect.y = (tiley + 1) * 2* 64 - playerRect.y;
		//		physicsSpeed.y = 0;
		//		playerRect.x = 2 * 64 * (tilex + 1) - playerRect.x;
		//	}
		//	else if (collisionType == CollisionType::AIR_SOLID)
		//	{
		//		playerRect.y = (tiley + 1) * 2 * 64 - playerRect.y;
		//		physicsSpeed.y = 0;
		//	}
		//	else if (collisionType == CollisionType::SOLID_AIR)
		//	{
		//		physicsSpeed.y -= 5.0f;
		//		if (physicsSpeed.y <= 0) {
		//			physicsSpeed.y = 0;
		//		}
		//		playerRect.x = 2 * 64 * (tilex + 1) - playerRect.x;
		//	}
		//}
		//else if (!positiveSpeedX && positiveSpeedY)	//checking the left and bottom corners
		//{
		//	CollisionType collisionType = GetCollisionType(GetTileProp(tilex, tiley, "Collider"), GetTileProp(tilex + 1, tiley + 1, "Collider"));
		//	if (collisionType == CollisionType::SOLID_SOLID)
		//	{
		//		playerRect.y = tiley * 2 * 64 - playerRect.y;
		//		physicsSpeed.y = 0;
		//		playerRect.x = 2 * 64 * (tilex + 1) - playerRect.x;
		//		physicsSpeed.x = 0;
		//		jumps = 2;
		//	}
		//	else if (collisionType == CollisionType::AIR_SOLID)
		//	{
		//		playerRect.y = tiley * 2 * 64 - playerRect.y;
		//		physicsSpeed.y = 0;
		//		physicsSpeed.x = 0;
		//		jumps = 2;
		//	}
		//	else if (collisionType == CollisionType::SOLID_AIR)
		//	{
		//		physicsSpeed.y -= 5.0f;
		//		if (physicsSpeed.y <= 0) {
		//			physicsSpeed.y = 0;
		//		}
		//		playerRect.x = 2 * 64 * (tilex + 1) - playerRect.x;
		//	}
		//}

		// Dead
		if (GetTileProp(playerRect.x/64, playerRect.y/64, "Collider") == Collider::Type::PAIN)
		{
			if (!godLike)
			{
				currentAnimation = &ded;
				heDed = true;
			}
		}
	}
	return true;
}

bool Player::PostUpdate()
{
	// Draw everything --------------------------------------
	currentAnimation->Update();
	app->render->DrawTexture(playerTexture, playerRect.x, playerRect.y, &currentAnimation->GetCurrentFrame(), inverted);
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
	while (MapLayerList != NULL) {
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

//Player::CollisionType Player::GetCollisionType(int A, int B) const
//{
//	if (A == Collider::Type::SOLID && B == Collider::Type::SOLID) { return CollisionType::SOLID_SOLID; }
//	else if (A == Collider::Type::SOLID && B == Collider::Type::AIR) { return CollisionType::SOLID_AIR; }
//	else if (A == Collider::Type::AIR && B == Collider::Type::AIR) { return CollisionType::AIR_AIR; }
//	else if (A == Collider::Type::AIR && B == Collider::Type::SOLID) { return CollisionType::AIR_SOLID; }
//}

void Player::resolveCollisions(iPoint nextFrame, bool positiveSpeedY) {

	iPoint tiledPos(playerRect.x / 64, playerRect.y / 64);
	iPoint correctedPos;
	iPoint checkedPos;
	//LOG("past: %d,%d current: %d,%d\n", playerRect.x, playerRect.y, nextFrame.x, nextFrame.y);

	// X axis
	if (!inverted) { // right
		tiledPos.x = (playerRect.x + playerRect.w) / 64;
		int i = 0;
		while (GetTileProp(tiledPos.x + i, tiledPos.y, "Collider") == Collider::Type::AIR && i < 5) {
			i++;
		}
		i--;
		correctedPos.x = MIN(nextFrame.x - playerRect.x, (tiledPos.x + i) * 64 - playerRect.x);
	}
	else { // left
		int i = 0;
		while (GetTileProp(tiledPos.x - i, tiledPos.y, "Collider") == Collider::Type::AIR && i < 5) {
			i++;
		}
		i--;
		correctedPos.x = -MIN(playerRect.x - nextFrame.x, playerRect.x - (tiledPos.x - i) * 64);
	}

	// Y axis
	if (positiveSpeedY) {
		tiledPos.y = (playerRect.y + playerRect.h) / 64;
		int i = 0;
		while (GetTileProp(tiledPos.x, tiledPos.y + i, "Collider") == Collider::Type::AIR && i < 5) {
			i++;
		}
		i--;
		correctedPos.y = MIN(nextFrame.y - playerRect.y, (tiledPos.y + i) * 64 - playerRect.y);
	}
	else {
		int i = 0;
		while (GetTileProp(tiledPos.x, tiledPos.y - i, "Collider") == Collider::Type::AIR && i < 5) {
			i++;
		}
		i--;
		correctedPos.y = -MIN(playerRect.y - nextFrame.y, playerRect.y - (tiledPos.y - i) * 64);
	}

	playerRect.x += correctedPos.x;
	playerRect.y += correctedPos.y;


	if (GetTileProp(playerRect.x / 64, playerRect.y / 64 + 1, "Collider") == Collider::Type::SOLID) {

		/*if (isJumping) {
			currentAnimation = &jumpLand;
		}*/
		physicsSpeed.y = 0.0f;
		jumps = 2;
	}
	else if (GetTileProp(playerRect.x / 64, playerRect.y / 64, "Collider") == Collider::Type::SOLID) {
		physicsSpeed.y = 0.0f;
	}
}
