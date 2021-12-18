#pragma once
#include "p2Point.h"
#include "Animation.h"
#include "SString.h"
#include "HandGun.h"
#include "p2List.h"

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
class Application;
struct SDL_Texture;
class PhysObject;
class EntityManager;
class Weapon;
class Entity
{
public:
	Entity(){}
	Entity(EntityType type_, p2Point<float> position_, Team team_, Application* app) : type(type_), position(position_), team(team_), app_(app){}

	virtual void Update(float dt){}
	virtual void Draw(SDL_Texture* tex){}

	p2Point<float> GetPos() { return position; }

public:
	Application* app_;
	EntityType type;
	bool isSelected;
	bool setPendingToDelete;
	p2Point<float> position;
	SDL_Texture* sprite;
	PhysObject* pbody;
	Animation anim;
	Team team;
	SString name;
	int health;
	p2List_item<Weapon*>* currentWeapon;
	p2List<Weapon*> guns;
};