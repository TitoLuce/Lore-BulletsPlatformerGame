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

#include "Input.h"

#include "SDL/include/SDL_scancode.h"

Player::Player() {
	name.Create("titleScreen");
	
}

Player::~Player() {}

// Load assets
bool Player::Start() {
	app->transition->TransitionStep(nullptr, this, true, 1200.0f);
	playerTexture = app->tex->Load("Assets/PlayerSprites.png");

	//Reset animations
	moving.Reset();
	ded.Reset();

	speedX = 16.0f;
	x = 1600;
	y = 5120;

	heDed = false;
	inverted = false;
	godLike = false;
	return true;
}

bool Player::Awake(pugi::xml_node&) {
	
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

	doubleJumping.PushBack({ 0,576,64,64 });
	doubleJumping.PushBack({ 64,576,64,64 });
	doubleJumping.PushBack({ 128,576,64,64 });
	doubleJumping.SetSpeed(0.06f);

	ded.PushBack({ 0,0,64,64 });
	ded.PushBack({ 64,0,64,64 });
	ded.PushBack({ 128,0,64,64 });
	ded.PushBack({ 192,0,64,64 });
	ded.PushBack({ 256,0,64,64 });
	ded.PushBack({ 320,0,64,64 });
	ded.PushBack({ 384,0,64,64 });
	ded.SetSpeed(0.08f);

	jumpDown.PushBack({ 0,320,64,64 });
	jumpDown.PushBack({ 64,320,64,64 });
	jumpDown.PushBack({ 128,320,64,64 });
	jumpDown.SetSpeed(0.08f);

	return true;
}






bool Player::PreUpdate() 
{
	return true;
}

bool Player::Update(float dt)
{


	if (speedY >= 0) {
		positiveSpeedY = true;
	}
	else if (speedY < 0) {
		positiveSpeedY = false;
	}


	//Movement and animation
	if (!heDed)
	{
		if (speedY <= 0)
		{
			currentAnimation = &idle;
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
		{

			inverted = false;

			if (app->input->GetKey(SDL_SCANCODE_SPACE) != KEY_REPEAT)
			{
				currentAnimation = &moving;
			}

			x += speedX;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
		{

			if (!inverted)
			{
				inverted = true;
			}

			if (app->input->GetKey(SDL_SCANCODE_SPACE) != KEY_REPEAT)
			{
				currentAnimation = &moving;
			}
			x -= speedX;
		}
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT)
		{
			currentAnimation = &jumping;
			if (jumps == 2)
			{
				speedY = 16.0f;
			}
			if (jumps == 1)
			{
				speedY = 12.0f;
			}

			y -= speedY;
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			currentAnimation = &jumpDown;
			//Temporary, will be godmode later
			speedY = 16.0f;
			y += speedY;
		}

		playerPhysics.UpdatePhysics(y, physicsSpeed.y);


		int x = playerRect.x / 64;
		int y = playerRect.y / 64;
		if (x < 0)
		{
			x = 0;
		}
		if (y < 0)
		{
			y = 0;
		}


		if (positiveSpeedX && positiveSpeedY)	//bottom right corner
		{
			/*
			CollisionType collisionType = GetCollisionType(GetTileProperty(x, y + 1, "CollisionId"), GetTileProperty(x + 1, y, "CollisionId"));
			if (collisionType == CollisionType::DoubleSolid)
			{
				playerRect.y = y * 2 * 64 - playerRect.y;
				speed.y = 0;
				if (isJumping)
				{
					currentAnimation = &jumpLand;
				}
				playerRect.x = 2 * 64 * (x + 1) - 64 * 2 - playerRect.x;
				LOG("BottomRight - DoubleSolid");
			}
			else if (collisionType == CollisionType::AirSolid)
			{
				playerRect.x = 2 * 64 * (x + 1) - 64 * 2 - playerRect.x;
				LOG("BottomRight - AirSolid");
			}
			else if (collisionType == CollisionType::SolidAir)
			{
				playerRect.y = y * 2 * 64 - playerRect.y;
				speed.y = 0;
				if (isJumping)
				{
					currentAnimation = &jumpLand;
				}
				LOG("BottomRight - SolidAir");
		}*/
		}
		else if (positiveSpeedX && !positiveSpeedY)	//top right corner
		{/*
			CollisionType collisionType = GetCollisionType(GetTileProperty(x, y, "CollisionId"), GetTileProperty(x + 1, y + 1, "CollisionId"));
			if (collisionType == CollisionType::DoubleSolid)
			{
				playerRect.y = (y + 1) * 2 * 64 - playerRect.y;
				speed.y = 0;
				playerRect.x = 2 * 64 * (x)-playerRect.x;
				LOG("TopRight - DoubleSolid");
			}
			else if (collisionType == CollisionType::AirSolid)
			{
				playerRect.x = 2 * 64 * (x)-playerRect.x;
				LOG("TopRight - AirSolid");
			}
			else if (collisionType == CollisionType::SolidAir)
			{
				playerRect.y = (y + 1) * 2 * 64 - playerRect.y;
				speed.y = 0;
				LOG("TopRight - SolidAir");
			}*/
		}
		else if (!positiveSpeedX && !positiveSpeedY)	//top left corner
		{
			/*CollisionType collisionType = GetCollisionType(GetTileProperty(x, y + 1, "CollisionId"), GetTileProperty(x + 1, y, "CollisionId"));
			if (collisionType == CollisionType::DoubleSolid)
			{
				playerRect.y = (y + 1) * 2 * 64 - playerRect.y;
				speed.y = 0;
				playerRect.x = 2 * 64 * (x + 1) - playerRect.x;
				LOG("TopLeft - DoubleSolid");
			}
			else if (collisionType == CollisionType::AirSolid)
			{
				playerRect.y = (y + 1) * 2 * 64 - playerRect.y;
				speed.y = 0;
				LOG("TopLeft - AirSolid");
			}
			else if (collisionType == CollisionType::SolidAir)
			{
				playerRect.x = 2 * 64 * (x + 1) - playerRect.x;
				LOG("TopLeft - SolidAir");
			}*/
		}
		else if (!positiveSpeedX && positiveSpeedY)	//bottom left corner
		{
			/*CollisionType collisionType = GetCollisionType(GetTileProperty(x, y, "CollisionId"), GetTileProperty(x + 1, y + 1, "CollisionId"));
			if (collisionType == CollisionType::DoubleSolid)
			{
				playerRect.y = y * 2 * 64 - playerRect.y;
				speed.y = 0;
				if (isJumping)
				{
					currentAnimation = &jumpLand;
				}
				playerRect.x = 2 * 64 * (x + 1) - playerRect.x;
				LOG("BottomLeft - DoubleSolid");
			}
			else if (collisionType == CollisionType::AirSolid)
			{
				playerRect.y = y * 2 * 64 - playerRect.y;
				speed.y = 0;
				if (isJumping)
				{
					currentAnimation = &jumpLand;
				}
				LOG("BottomLeft - AirSolid");
			}
			else if (collisionType == CollisionType::SolidAir)
			{
				playerRect.x = 2 * 64 * (x + 1) - playerRect.x;
				LOG("BottomLeft - SolidAir");
			}*/

		}







	}
	
	playerRect = { x , y , w, h };
	return true;
}

// Update: draw background
bool Player::PostUpdate() {
	// Draw everything --------------------------------------

	currentAnimation->Update();
	app->render->DrawTexture(playerTexture, x, y, &currentAnimation->GetCurrentFrame(), inverted);
	return true;
}

bool Player::CleanUp() {
	app->tex->UnLoad(playerTexture);
	return true;
}


void Player::Init()
{
	active = false;
}

int Player::GetColliderId(int x, int y) const
{
	int ret;
	// MapLayer		
	ListItem <MapLayer*>* MapLayerList = app->map->data.layers.start;
	SString layerName;
	
	layerName = "Collisions";
	
	while (MapLayerList != NULL) {
		if (MapLayerList->data->name == layerName) {
			break;
		}
		MapLayerList = MapLayerList->next;
	}

	// Tileset		
	ListItem <TileSet*>* TilesetList = app->map->data.tilesets.start;
	SString tilesetName;
	
	tilesetName = "MetaData";
	
	while (TilesetList != NULL) {
		if (TilesetList->data->name == tilesetName) {
			break;
		}
		TilesetList = TilesetList->next;
	}

	// Gets CollisionId
	int id = (int)(MapLayerList->data->Get(x, y) - TilesetList->data->firstgid);	//returns id of the tile
	if (id < 0) {
		ret = 0;
		return ret;
	}
	Tile* currentTile = TilesetList->data->GetPropList(id);
	ret = currentTile->properties.GetProperty("CollisionId", 0);	
	//LOG("%d - %d", id, ret);

	return ret;
}

Player::CollisionType Player::GetCollisionType(int A, int B) const
{
	/*if (A == Collider::TYPE::SOLID && B == Collider::TYPE::SOLID) {
		return CollisionType::DoubleSolid;
	}
	else if (A == Collider::TYPE::SOLID && B == Collider::TYPE::AIR) {
		return CollisionType::SolidAir;
	}
	else if (A == Collider::TYPE::AIR && B == Collider::TYPE::AIR) {
		return CollisionType::DoubleAir;
	}
	else if (A == Collider::TYPE::AIR && B == Collider::TYPE::SOLID) {
		return CollisionType::AirSolid;
	}*/


	return CollisionType::SOLID_AIR;
}
