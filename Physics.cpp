#include "Physics.h"
#include <math.h>

float Distance(int x1, int y1, int x2, int y2)
{
	return sqrtf(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)));
}

PhysObject::PhysObject()
{
}

PhysObject::PhysObject(Shape shape_, Type type_, int x_, int y_, float w_, float h_)
{
	shape = shape_;
	type = type_;
	x = x_;
	y = y_;
	w = w_;
	h = h_;
	if (shape == Shape::CIRCLE)
	{
		r = w / 2;
	}

}

PhysObject::~PhysObject()
{
}

bool PhysObject::Intersects(PhysObject o)
{
	float normalx;
	float normaly;
	float dx, dy;

	bool ret = false;
	if (shape == Shape::RECTANGLE && o.shape == Shape::RECTANGLE)
	{
		ret = (x < o.x + o.w &&
			x + w > o.x &&
			y < o.y + o.h &&
			h + y > o.y);



	}

	if (shape == Shape::CIRCLE && o.shape == Shape::CIRCLE)
	{
		if (Distance(x, y, o.x, o.y) < (r + o.r)) ret = true;
	}

	if (shape == Shape::CIRCLE && o.shape == Shape::RECTANGLE)
	{

	}



	return ret;
}

void PhysObject::Reposition(float x, float y)
{

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
		p2Point<float> speed;
		speed.x = o->data->vx - atmosphere.windx;
		speed.y = o->data->vy - atmosphere.windy;
		float fdrag = 0.5 * atmosphere.density * speed.x * o->data->surface * o->data->cd;
		float flift = 0.5 * atmosphere.density * speed.y * o->data->surface * o->data->cl;
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