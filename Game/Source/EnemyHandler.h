#ifndef __ENEMY_HANDLER_H
#define __ENEMY_HANDLER_H

#include "Module.h"
#include "Animation.h"
#include "Physics.h"
#include "Collisions.h"
#include "Enemy.h"

#include "SDL/include/SDL.h"


#define MAX_ENEMIES 100

struct EnemySpawnpoint
{
	EnemyType type = EnemyType::NO_TYPE;
	int x, y;
};


class Enemy;
enum EnemyType;
struct SDL_Texture;

class EnemyHandler : public Module {
public:
	//Constructor
	EnemyHandler();

	//Destructor
	~EnemyHandler();

	void Init();


	bool Awake(pugi::xml_node& config);

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start();


	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt);

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

	bool CleanUp();


	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

	// Called when an enemi collider hits another collider
	// The enemy is destroyed and an explosion particle is fired
	void OnCollision(Collider* c1, Collider* c2);


	// Add an enemy into the queue to be spawned later
	bool AddEnemy(EnemyType type, int x, int y, uint i = -1);



	// Iterates the queue and checks for camera position
	void HandleEnemiesSpawn();

	// Destroys any enemies that have moved outside the camera limits
	void HandleEnemiesDespawn();


	SDL_Rect enemyRect;
	/*SDL_Rect slimeRect = { 0 , 0 , 64, 64 };
	SDL_Rect flyRect = { 0 , 0 , 64, 64 };*/
	//SDL_Rect goblinRect = { 0 , 0 , 64, 64 };


private:
	// Spawns a new enemy using the data from the queue
	void SpawnEnemy(const EnemySpawnpoint& info);

	// A queue with all spawn points information
	EnemySpawnpoint spawnQueue[MAX_ENEMIES];

	// All spawned enemies in the scene
	Enemy* enemies[MAX_ENEMIES] = { nullptr };

	SDL_Texture* slimeTexture = nullptr;
	SDL_Texture* flyTexture = nullptr;
	//SDL_Texture* goblinTexture = nullptr;

	


	////Goblin animations
	//Animation goblinMoving;
	//Animation goblinDed;
	//Animation goblinIdle;
	//Animation goblinHit;

	//Checks if animation is inverted
	
	
	//bool goblinInverted = false;

	//Enemy states
	bool slimeIsDed = false;
	bool flyIsDed = false;
	//bool goblinIsDed = false;
	/*int goblinHP = 2;
	bool goblinIsHit = false;
	int goblinCounter = 0;*/

	//Pointer to current enemy animations
	/*
	Animation* currentFlyAnimation = &flyIdle;*/
	//Animation* currentGoblinAnimation = &slimeIdle;

	//Player SFX
	unsigned int deathSFX;
	unsigned int hitSFX;
	bool alreadyPlayed = false;
};
#endif