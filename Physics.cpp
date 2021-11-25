#include "Physics.h"
#include <math.h>
float Distance(int x1, int y1, int x2, int y2)
{
	// Calculating distance
	return sqrtf(powf(x2 - x1, 2) +
		powf(y2 - y1, 2) * 1.0);
}
bool Intersects(PhysObject* o, PhysObject* c)
{
	float normalx;
	float normaly;
	float dx, dy;

	bool ret = false;
	if (o->shape == Shape::RECTANGLE && c->shape == Shape::RECTANGLE)
	{
		ret = (o->x < c->x + c->w &&
			o->x + o->w > c->x &&
			o->y < c->y + c->h &&
			o->y + o->h > c->y);
	}

	if (o->shape == Shape::CIRCLE && c->shape == Shape::CIRCLE)
	{
		if (Distance(o->x, o->y, c->x, c->y) < (o->r + c->r)) ret = true;
	}

	if (o->shape == Shape::CIRCLE && c->shape == Shape::RECTANGLE)
	{
		p2Point<float> circleDistance;
		circleDistance.x = fabsf(o->x - c->x);
		circleDistance.y = fabsf(o->y - c->y);

		if (circleDistance.x > (c->w / 2 + o->r)) { return false; }
		if (circleDistance.y > (c->h / 2 + o->r)) { return false; }

		if (circleDistance.x <= (c->w / 2)) { return true; }
		if (circleDistance.y <= (c->h / 2)) { return true; }

		float cornerDistance_sq = powf((circleDistance.x - c->w / 2), 2) +
			powf((circleDistance.y - c->h / 2), 2);

		return (cornerDistance_sq <= (powf(o->r, 2)));
	}



	return ret;
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
		if (o->data->physics_enabled && o->data->type == Type::DYNAMIC)
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
			float speed = sqrtf(powf((o->data->vx - atmosphere.windx), 2) + powf(o->data->vy - atmosphere.windy,2));
			float fdrag = 0.5 * atmosphere.density * speed * speed * o->data->surface * o->data->cd;
			float flift = 0.5 * atmosphere.density * speed * speed  * o->data->surface * o->data->cl;
			float fdx = -fdrag; // Let's assume Drag is aligned with x-axis (in your game, generalize this) Opuesta al vector speed = normalizar speed y multiplicar
			float fdy = flift; // Let's assume Lift is perpendicular with x-axis (in your game, generalize this)  Perpendicular al drag si la shape tiene lift
			// Add gravity force to the total accumulated force of the ball
			//o->data->fx += fdx;
			//o->data->fy += fdy;

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
			case Integrator::VERLET:
				IntegratorVelocityVerlet(o->data, dt);
				break;
			case  Integrator::SEULER:
				IntegratorVelocitySymplecticEuler(o->data, dt);
				break;
			case  Integrator::IEULER:
				IntegratorVelocityImplicitEuler(o->data, dt);
				break;
			}
		}
		
		if (o->data->type == Type::STATIC)
		{
			o->data->fx = o->data->fy = o->data->vx = o->data->vx = 0.0;
			o->data->ax = o->data->ay = 0.0;
		}
		//Collision Solver
		p2List_item<PhysObject*>* c = objects.getFirst();
		while (c != NULL)
		{
			if (c->data != o->data)
			{
				if (Intersects(o->data, c->data))
				{
					printf("\ncollision 1: %s 2: %s\n", o->data->name.GetString(), c->data->name.GetString());
					if (o->data->object == ObjectType::PORTAL && c->data->object != ObjectType::PORTAL && c->data->hasEnteredAPortal == false)
					{
						portal->Teletransport(o->data, c->data);
						printf("\nPortal %s, %s", o->data->name.GetString(), c->data->name.GetString());
						c->data->hasEnteredAPortal = true;
						break;
					}
					else
					{
						float fDistance = sqrtf((o->data->x - c->data->x) * (o->data->x - c->data->x) + (o->data->y - c->data->y) * (o->data->y - c->data->y));

						// Normal
						float nx = (c->data->x - o->data->x) / fDistance;
						float ny = (c->data->y - o->data->y) / fDistance;

						// Tangent
						float tx = -ny;
						float ty = nx;

						// Dot Product Tangent
						float dpTan1 = o->data->vx * tx + c->data->vy * ty;
						float dpTan2 = c->data->vx * tx + c->data->vy * ty;

						// Dot Product Normal
						float dpNorm1 = o->data->vx * nx + o->data->vy * ny;
						float dpNorm2 = c->data->vx * nx + c->data->vy * ny;

						// Conservation of momentum in 1D
						float m1 = (dpNorm1 * (o->data->mass - c->data->mass) + 2.0f * c->data->mass * dpNorm2) / (o->data->mass + c->data->mass);
						float m2 = (dpNorm2 * (c->data->mass - o->data->mass) + 2.0f * o->data->mass * dpNorm1) / (o->data->mass + c->data->mass);

						// Update ball velocities
						o->data->vx = tx * dpTan1 + nx * m1;
						o->data->vy = ty * dpTan1 + ny * m1;
						c->data->vx = tx * dpTan2 + nx * m2;
						c->data->vy = ty * dpTan2 + ny * m2;
						////TODO COLLISION FORCES STUFF


						
					}
				}
				
			}
			
			c = c->next;
		}
		
		
		o = o->next;
	}


	return true;
}
// Integration scheme: Velocity Verlet
// You should modularise all your algorithms into subroutines. Including the ones to compute forces.
void Physics::IntegratorVelocityVerlet(PhysObject* obj, float dt)
{
	obj->x += obj->vx * dt + 0.5 * obj->ax * dt * dt;
	obj->y += obj->vy * dt + 0.5 * obj->ay * dt * dt;
	obj->vx += obj->ax * dt;
	obj->vy += obj->ay * dt;
	if (obj->vy >= 300 || obj->vy <= -300)
	{
		obj->vy = 0;
	}
	if (obj->vx >= 300 || obj->vx <= -300)
	{
		obj->vx = 0;
	}
}
void Physics::IntegratorVelocitySymplecticEuler(PhysObject* obj, float dt)
{
	       // Gravity will always act on the body
	obj->vx += obj->ax * dt;
	obj->vy += obj->ay * dt;
	obj->x += obj->vx * dt;
	obj->y += obj->vy * dt;
	if (obj->vy >= 300 || obj->vy <= -300)
	{
		obj->vy = 0;
	}
	if (obj->vx >= 300 || obj->vx <= -300)
	{
		obj->vx = 0;
	}
}
void Physics::IntegratorVelocityImplicitEuler(PhysObject* obj, float dt)
{
	obj->x += obj->vx * dt;
	obj->y += obj->vy * dt;
	obj->vx += obj->ax * dt;
	obj->vy += obj->ay * dt;
	if (obj->vy >= 300 || obj->vy <= -300)
	{
		obj->vy = 0;
	}
	if (obj->vx >= 300 || obj->vx <= -300)
	{
		obj->vx = 0;
	}
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

void Physics::DestroyObject(PhysObject* obj)
{
	
	p2List_item<PhysObject*>* a = objects.findNode(obj);
	p2List_item<PhysObject*>* b = objects.findNode(obj);

	objects.del(a);
	delete a;
	delete obj;
}

