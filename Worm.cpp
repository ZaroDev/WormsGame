#include "Worm.h"
#include "ModulePhysics.h"


Worm::Worm(p2Point<float> position_, Team team_) : Entity(EntityType::WORM, position_, team_)
{
	name.Create("worm");
	pbody = new PhysObject();
	pbody->mass = 1000.0f;
	pbody->x = position.x;
	pbody->y = position.y;
	pbody->type = Type::DYNAMIC;
	pbody->shape = Shape::RECTANGLE;
	pbody->h = 20;
	pbody->w = 20;
	health = 100;
	pbody->restitution = 0.1f;
	pbody->SetLimit(Vector2d(300.0f, 300.0f));
}

Worm::~Worm()
{
}

void Worm::Update(float dt)
{
	position.x = pbody->x;
	position.y = pbody->y;
}

void Worm::Draw()
{
}

void Worm::Select()
{
	isSelected = true;
}

void Worm::UnSelect()
{
	isSelected = false;
}
