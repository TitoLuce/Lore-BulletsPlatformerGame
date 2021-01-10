#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"

#include "Animation.h"
#include "Physics.h"
#include "DynArray.h"

class Collider;


enum class EntityType
{
    PLAYER,
    ENEMY,
    ITEM,
    MAP,
    UNKNOWN
};


enum EnemyType
{
    NO_TYPE,
    GROUND,
    FLYING
};


class Entity
{
public:

    Entity(int x, int y, EntityType type, EnemyType eType = EnemyType::NO_TYPE) : type(type), eType(eType) {}

    virtual bool Update(float dt)
    {
        return true;
    }

    // Blit
    virtual bool Draw()
    {
        return true;
    }

    // Collision response
    virtual void OnCollision(Collider* c1, Collider* c2)
    {}

public:

    EntityType type;
    SDL_Rect entityRect;
    Collider* collider;
    Physics physics;
    iPoint nextPos;


    int playerSpawnpointX = 1600;
    int playerSpawnpointY = 5120;

    bool pendingToDelete = false;

    Animation* currentAnim = nullptr;
    bool invert = false;

    //bool active = true;

    iPoint spawnPos;

    bool heDed;

    //SDL_Texture* texture;

    EnemyType eType;

};

#endif // __ENTITY_H__