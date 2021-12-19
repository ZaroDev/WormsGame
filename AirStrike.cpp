#include "AirStrike.h"
#include "ModuleInput.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "Physics.h"
AirStrike::AirStrike(Application* app_, Module* listener_, Entity* ent_) : Weapon(app_, listener_, ent_)
{
	name.Create("airstrike");
	ammo = 1;
	id = 1;
}

AirStrike::~AirStrike()
{
}

void AirStrike::Use(Vector2d position)
{
	if (ammo > 0)
	{
		ammo--;
		//TODO shot
		int mx, my;
		mx = app->input->GetMouseX();
		my = app->input->GetMouseY();


		PhysObject* bomb = new PhysObject();

		bomb->x = mx;
		bomb->y = 0;
		bomb->mass = 5000.0f;
		bomb->shape = Shape::RECTANGLE;
		bomb->w = 20;
		bomb->h = 50;
		bomb->listener = listener;
		bomb->restitution = 0.1f;
		bomb->name.Create("Ground2");
		bomb->type = Type::DYNAMIC;
		bomb->object = ObjectType::BOMB;
		bomb->SetLimit(Vector2d(300.0f, 300.0f));
		app->physics->world.CreateObject(bomb);


	}
}
