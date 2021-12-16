#pragma once
#include "p2Point.h"
#include "Animation.h"
#include "SString.h"

enum class EntityType
{
	UNKNOWN = -1,
	WORM
};
enum class Team
{
	UNKNOWN = -1,
	RED,
	BLUE
};
struct SDL_Texture;
class PhysObject;
class EntityManager;
class Entity
{
public:
	Entity(){}
	Entity(EntityType type_, p2Point<float> position_, Team team_) : type(type_), position(position_), team(team_){}

	virtual void Update(float dt){}
	virtual void Draw(){}

	p2Point<float> GetPos() { return position; }

public:
	EntityType type;
	bool setPendingToDelete;
	p2Point<float> position;
	SDL_Texture* sprite;
	PhysObject* pbody;
	Animation anim;
	Team team;
	SString name;
	int health;
};