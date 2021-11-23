#include "Physics.h"
#include <math.h>

PhysObject::PhysObject()
{
}

PhysObject::~PhysObject()
{
}

float PhysObject::Speed(float fx, float fy)
{
	return sqrtf(powf(fx, 2) + powf(fy, 2));
}

Physics::Physics()
{
	 
}

Physics::~Physics()
{
}

bool Physics::Start(Integrator _integrator, float gx, float gy)
{
	bool ret = true;
	integrator = _integrator;
	gravityX = gx;
	gravityY = gy;
	return ret;
}

bool Physics::Update(float dt)
{
	p2List_item<PhysObject*>* o = objects.getFirst();
	while (o != NULL)
	{
		// Step #0: Reset total acceleration and total accumulated force of the ball (clear old values)
		o->data->fx = o->data->fy = 0.0;
		o->data->ax = o->data->ay = 0.0;

		// Step #1: Compute forces

			// Compute Gravity force
		float fgx = o->data->mass * gravityX;
		float fgy = o->data->mass * gravityY; // Let's assume gravity is constant and downwards

		// Add gravity force to the total accumulated force of the ball
		o->data->fx += fgx;
		o->data->fy += fgy;

		// Compute Aerodynamic Lift & Drag forces
		float speed = o->data->Speed(o->data->vx - atmosphere.windx, o->data->vy - atmosphere.windy);
		float fdrag = 0.5 * atmosphere.density * speed * speed * o->data->surface * o->data->cd;
		float flift = 0.5 * atmosphere.density * speed * speed * o->data->surface * o->data->cl;
		float fdx = -fdrag; // Let's assume Drag is aligned with x-axis (in your game, generalize this)
		float fdy = flift; // Let's assume Lift is perpendicular with x-axis (in your game, generalize this) 
		// Add gravity force to the total accumulated force of the ball
		o->data->fx += fdx;
		o->data->fy += fdy;

		// Other forces
		// ...

	// Step #2: 2nd Newton's Law: SUM_Forces = mass * accel --> accel = SUM_Forces / mass
		o->data->ax = o->data->fx / o->data->mass;
		o->data->ay = o->data->fy / o->data->mass;

		// Step #3: Integrate --> from accel to new velocity & new position. 
		// We will use the 2nd order "Velocity Verlet" method for integration.
		// You can also move this code into a subroutine: integrator_velocity_verlet(ball, dt);
		switch (integrator)
		{
		case NONE:
			break;
		case VERLET:
			IntegratorVelocityVerlet(o->data, dt);
			break;
		default:
			break;
		}
		// Step #4: solve collisions
		//if (o->data->y < ground.y)
		//{
		//	// For now, just stop the ball when it reaches the ground.
		//	o->data->vx = o->data->vy = 0.0;
		//	o->data->ax = o->data->ay = 0.0;
		//	o->data->fx = o->data->fy = 0.0;
		//	o->data->physics_enabled = false;
		//}
		o = o->next;   
	}


	return true;
}
// Integration scheme: Velocity Verlet
// You should modularise all your algorithms into subroutines. Including the ones to compute forces.
void Physics::IntegratorVelocityVerlet(PhysObject* obj, double dt)
{
	obj->x += obj->vx * dt + 0.5 * obj->ax * dt * dt;
	obj->y += obj->vy * dt + 0.5 * obj->ay * dt * dt;
	obj->vx += obj->ax * dt;
	obj->vy += obj->ay * dt;
}
bool Physics::CleanUp()
{
	bool ret = true;
	objects.clear();
	return true;
}

void Physics::CreateObject(PhysObject* obj)
{
	objects.add(obj);
}


