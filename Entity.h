#pragma once
#include "p2Point.h"
#include "Animation.h"

enum EntityType
{
	UNKNOWN = -1,
	WORM_T1,
	WORM_T2
};
struct SDL_Texture;
class PhysObject;
class EntityManager;
class Entity
{
public:
	Entity(){}
	Entity(EntityType type_, p2Point<float> position_) : type(type_), position(position_){}

	virtual void Update(float dt){}
	virtual void Draw(){}

public:
	EntityType type;
	bool setPendingToDelete;
	p2Point<float> position;
	SDL_Texture* sprite;
	PhysObject* pbody;
	Animation anim;
	int health;
};