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

Player::Player() {
	name.Create("titleScreen");
	
}

Player::~Player() {}

// Load assets
bool Player::Start() {
	app->transition->TransitionStep(nullptr, this, true, 1200.0f);
	playerTexture = app->tex->Load("Assets/PlayerSprites.png");

	playerRect = { 1600 , 5120 , idle.GetCurrentFrame().w, idle.GetCurrentFrame().h };


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
	ded.SetLoop(false);

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


	if (physicsSpeed.y >= 0) positiveSpeedY = true;
	else if (physicsSpeed.y < 0) positiveSpeedY = false;
	

	 if (physicsSpeed.x >= 0) positiveSpeedX = true;
	 else if (physicsSpeed.x < 0)positiveSpeedX = false;





	//Movement and animation
	if (!heDed)
	{
		if (physicsSpeed.y <= 0)
		{
			currentAnimation = &idle;
		}


		if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		{
			heDed = true;
		}


		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
		{
			
			inverted = false;

			if (app->input->GetKey(SDL_SCANCODE_SPACE) != KEY_REPEAT)
			{
				currentAnimation = &moving;
			}

			playerRect.x += 5;
			positiveSpeedX = true;
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
			playerRect.x -= 5;
			positiveSpeedX = false;
		}
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && app->input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT)
		{
			currentAnimation = &jumping;
			if (jumps == 2)
			{
				physicsSpeed.y += 1.0f;
			}
			if (jumps == 1)
			{
				physicsSpeed.y += 1.0f;
			}

			 physicsSpeed.y = -500;
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			currentAnimation = &jumpDown;
			//Temporary, will be godmode later
			physicsSpeed.y = 16.0f;
			playerRect.y += 5;
		}




		playerPhysics.UpdatePhysics(playerRect.x, playerRect.y, physicsSpeed.x, physicsSpeed.y);




		//collision 
		int tilex = playerRect.x / 64;
		int tiley = playerRect.y / 64;
		if (tilex < 0)
		{
			tilex = 0;
		}
		if (tiley < 0)
		{
			tiley = 0;
		}

		if (positiveSpeedX && positiveSpeedY)	//checking the bottom right corner
		{
			
			CollisionType collisionType = GetCollisionType(GetTileProp(tilex, tiley + 1, "Collider"), GetTileProp(tilex + 1, tiley, "Collider"));
			if (collisionType == CollisionType::SOLID_SOLID)
			{
				playerRect.y = tiley * 2 * 64 - playerRect.y;
				physicsSpeed.y = 0;
				
				playerRect.x = 2 * 64 * (tilex + 1) - 64 * 2 - playerRect.x;
				physicsSpeed.x = 0;

			}
			else if (collisionType == CollisionType::AIR_SOLID)
			{
				
				physicsSpeed.y -= 5.0f;
				if (physicsSpeed.y <= 0) {
					physicsSpeed.y = 0;
				}
				playerRect.x = 2 * 64 * (tilex + 1) - 64 * 2 - playerRect.x;
				
			}
			else if (collisionType == CollisionType::SOLID_AIR)
			{
				playerRect.y = tiley * 2 * 64 - playerRect.y;
				physicsSpeed.y = 0;
				physicsSpeed.x = 0;
				
			}
			else {}
		}
		else if (positiveSpeedX && !positiveSpeedY)	//checking the top right corner
		{
			CollisionType collisionType = GetCollisionType(GetTileProp(tilex, tiley, "Collider"), GetTileProp(tilex + 1, tiley + 1, "Collider"));
			if (collisionType == CollisionType::SOLID_SOLID)
			{
				playerRect.y = (tiley + 1) * 2 * 64 - playerRect.y;
				physicsSpeed.y = 0;
				playerRect.x = 2 * 64 * (tilex)-playerRect.x;
				
			}
			else if (collisionType == CollisionType::AIR_SOLID)
			{
				physicsSpeed.y -= 5.0f;
				if (physicsSpeed.y <= 0) {
					physicsSpeed.y = 0;
				}
				
				playerRect.x = 2 * 64 * (tilex)-playerRect.x;
	
			}
			else if (collisionType == CollisionType::SOLID_AIR)
			{
				playerRect.y = (tiley + 1) * 2 * 64 - playerRect.y;
				physicsSpeed.y = 0;
				
			}
		}
		else if (!positiveSpeedX && !positiveSpeedY)	//checking the left top corner
		{
			CollisionType collisionType = GetCollisionType(GetTileProp(tilex, tiley + 1, "Collider"), GetTileProp(tilex + 1, tiley, "Collider"));
			if (collisionType == CollisionType::SOLID_SOLID)
			{
				playerRect.y = (tiley + 1) * 2* 64 - playerRect.y;
				physicsSpeed.y = 0;
				playerRect.x = 2 * 64 * (tilex + 1) - playerRect.x;
				
				
			}
			else if (collisionType == CollisionType::AIR_SOLID)
			{
				playerRect.y = (tiley + 1) * 2 * 64 - playerRect.y;
				physicsSpeed.y = 0;
				
			}
			else if (collisionType == CollisionType::SOLID_AIR)
			{
				physicsSpeed.y -= 5.0f;
				if (physicsSpeed.y <= 0) {
					physicsSpeed.y = 0;
				}
				playerRect.x = 2 * 64 * (tilex + 1) - playerRect.x;
			}
		}
		else if (!positiveSpeedX && positiveSpeedY)	//checking the left bottom corner
		{
			CollisionType collisionType = GetCollisionType(GetTileProp(tilex, tiley, "Collider"), GetTileProp(tilex + 1, tiley + 1, "Collider"));
			if (collisionType == CollisionType::SOLID_SOLID)
			{
				playerRect.y = tiley * 2 * 64 - playerRect.y;
				physicsSpeed.y = 0;
				playerRect.x = 2 * 64 * (tilex + 1) - playerRect.x;
				physicsSpeed.x = 0;
			}
			else if (collisionType == CollisionType::AIR_SOLID)
			{
				playerRect.y = tiley * 2 * 64 - playerRect.y;
				physicsSpeed.y = 0;
				physicsSpeed.x = 0;
			}
			else if (collisionType == CollisionType::SOLID_AIR)
			{
				physicsSpeed.y -= 5.0f;
				if (physicsSpeed.y <= 0) {
					physicsSpeed.y = 0;
				}
				playerRect.x = 2 * 64 * (tilex + 1) - playerRect.x;
				
			}

		}


		// Win condition
		if (GetTileProp(tilex, tiley, "Collider") == Collider::TYPE::END)
		{
				
			
		}

		// Dead
		if (GetTileProp(tilex, tiley, "Collider") == Collider::TYPE::PAIN)
		{


			currentAnimation = &ded;
			heDed = true;
		}

		

	}
	
	return true;
}

// Update: draw background
bool Player::PostUpdate() {
	// Draw everything --------------------------------------

	currentAnimation->Update();
	app->render->DrawTexture(playerTexture, playerRect.x, playerRect.y, &currentAnimation->GetCurrentFrame(), inverted);
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

int Player::GetTileProp(int x, int y, const char* prop) const
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

Player::CollisionType Player::GetCollisionType(int A, int B) const
{
	if (A == Collider::TYPE::SOLID && B == Collider::TYPE::SOLID) {
		return CollisionType::SOLID_SOLID;
	}
	else if (A == Collider::TYPE::SOLID && B == Collider::TYPE::AIR) {
		return CollisionType::SOLID_AIR;
	}
	else if (A == Collider::TYPE::AIR && B == Collider::TYPE::AIR) {
		return CollisionType::AIR_AIR;
	}
	else if (A == Collider::TYPE::AIR && B == Collider::TYPE::SOLID) {
		return CollisionType::AIR_SOLID;
	}

	//may more elseifs




}
