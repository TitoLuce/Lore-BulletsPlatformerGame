#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"

#include "List.h"


class Entity;


class EntityManager : public Module
{
public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(int x, int y, EntityType type, Entity* playerPointer = nullptr, EnemyType eType = EnemyType::NO_TYPE);
	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	bool UpdateAll(float dt, bool doLogic);


	// Collision response
	void OnCollision(Collider* c1, Collider* c2);


	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&);

public:

	List<Entity*> entities;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;

	bool doLogic = false;

	SDL_Texture* slimeTexture = nullptr;
	SDL_Texture* flyTexture = nullptr;
	SDL_Texture* playerTexture = nullptr;
	SDL_Texture* specialBarTexture = nullptr;
	SDL_Texture* coinTexture = nullptr;

	//Player SFX
	unsigned int jumpSFX;
	unsigned int doubleJumpSFX;
	unsigned int deathSFX;
	unsigned int coinSFX;
	unsigned int attackSFX;
	unsigned int specialSFX;
	unsigned int flagSFX;
	bool alreadyPlayed = false;



};

#endif // __ENTITYMANAGER_H__
