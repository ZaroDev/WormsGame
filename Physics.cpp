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

	if (o->shape == Shape::RECTANGLE && c->shape == Shape::CIRCLE)
	{
		p2Point<float> circleDistance;
		circleDistance.x = fabsf(c->x - o->x);
		circleDistance.y = fabsf(c->y - o->y);

		if (circleDistance.x > (o->w / 2 + c->r)) { return false; }
		if (circleDistance.y > (o->h / 2 + c->r)) { return false; }

		if (circleDistance.x <= (o->w / 2)) { return true; }
		if (circleDistance.y <= (o->h / 2)) { return true; }

		float cornerDistance_sq = powf((circleDistance.x - o->w / 2), 2) +
			powf((circleDistance.y - o->h / 2), 2);

		return (cornerDistance_sq <= (powf(c->r, 2)));
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

bool Physics::PreUpdate()
{
	bool ret = true;
	p2List_item<PhysObject*>* o = objects.getFirst();
	while (o != NULL)
	{
		if (o->data->setPendingToDelete)
		{
			objects.del(o);
			break;
		}
		o = o->next;
	}
	return ret;
}

bool Physics::Update(float dt)
{
	p2List_item<PhysObject*>* o = objects.getFirst();

	while (o != NULL)
	{
		if (o->data->physics_enabled)
		{
			switch (o->data->type)
			{
			case Type::DYNAMIC:
			{
				// Step #0: Reset total acceleration and total accumulated force of the ball (clear old values)
				o->data->f.x = o->data->f.y = 0.0;
				o->data->a.x = o->data->a.y = 0.0;

				// Step #1: Compute forces

					// Compute Gravity force
				float fgx = o->data->mass * gravityX;
				float fgy = o->data->mass * gravityY; // Let's assume gravity is constant and downwards

				// Add gravity force to the total accumulated force of the ball
				o->data->f.x += fgx;
				o->data->f.y += fgy;

				// Compute Aerodynamic Lift & Drag forces
				float speed = sqrtf(powf((o->data->v.x - atmosphere.windx), 2) + powf(o->data->v.y - atmosphere.windy, 2));
				if (o->data->v.x != 0)
				{
					float fdrag = 0.5 * atmosphere.density * speed * speed * o->data->surface * o->data->cd;
					float fdx = -fdrag; // Let's assume Drag is aligned with x-axis (in your game, generalize this) Opuesta al vector speed = normalizar speed y multiplicar
					o->data->f.x += fdx;
				}
				// Let's assume Lift is perpendicular with x-axis (in your game, generalize this)  Perpendicular al drag si la shape tiene lift

				if (o->data->v.y != 0)
				{
					float flift = 0.5 * atmosphere.density * speed * speed * o->data->surface * o->data->cl;
					float fdy = -flift;
					o->data->f.y += fdy;
				}



				// Other forces
				// ...
				if (o->data->isOnWater)
				{
					float bfx = o->data->mass * gravityX * (water->density / o->data->density);
					float bfy = o->data->mass * gravityY * (water->density / o->data->density);
					o->data->f.x -= bfx;
					o->data->f.y -= bfy;
					o->data->isOnWater = false;
				}

				// Step #2: 2nd Newton's Law: SUM_Forces = mass * accel --> accel = SUM_Forces / mass
				o->data->a.x = o->data->f.x / o->data->mass;
				o->data->a.y = o->data->f.y / o->data->mass;
				printf("\nfx: %f, fy: %f", o->data->f.x, o->data->f.y);
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
			}

			//Collision Solver
		p2List_item<PhysObject*>* c = objects.getFirst();

			while (c != NULL)
			{
				if (c->data != o->data)
				{
					if(c->data->type == Type::DYNAMIC)
					{ 
						if (Intersects(o->data, c->data))
						{
							printf("\ncollision 1: %s 2: %s\n", o->data->name.GetString(), c->data->name.GetString());
							if (o->data != water && c->data == water)
							{
								o->data->isOnWater = true;
								break;
							}
							else if (o->data->object == ObjectType::PORTAL && c->data->object != ObjectType::PORTAL)
							{
								portal->Teletransport(o->data, c->data);
								printf("\nPortal %s, %s", o->data->name.GetString(), c->data->name.GetString());
								break;
							}
							else
							{
								//2m2/m1+m2
								float mass1 = (2 * c->data->mass) / (o->data->mass + c->data->mass);
								float mass2 = (2 * o->data->mass) / (o->data->mass + c->data->mass);

								//dot(v1-v2,  x1-x2) / ||x1-x2||^2
								Vector2d x1;
								x1.x = o->data->x;
								x1.y = o->data->y;
								Vector2d x2;
								x2.x = c->data->x;
								x2.y = c->data->y;
								float dot1 = Vector2d::CrossProduct(o->data->v - c->data->v, x1 - x2) / powf(Vector2d::Magnitude(x1 - x2), 2.0f);
								float dot2 = Vector2d::CrossProduct(c->data->v - o->data->v, x2 - x1) / powf(Vector2d::Magnitude(x2 - x1), 2.0f);
								Vector2d mult1 = (x1 - x2);
								Vector2d mult2 = (x2 - x1);

								mult1 *= dot1 * mass1;
								mult2 *= dot2 * mass2;

								o->data->v = o->data->v - (mult1 * -1) / 2;
								c->data->v = c->data->v - (mult2 * -1) / 2;

								printf("\nExpected Vel x: %f, y %f", o->data->v.x, o->data->v.y);
							}
						}
					}
					else
					{
						//m1-m2 / m1+m2
						float mass1 = (o->data->mass - c->data->mass) / (o->data->mass + c->data->mass);
						float mass2 = (c->data->mass - o->data->mass) / (c->data->mass + o->data->mass);
						//2m2 / m1+m2
						float masst1 = (2 * c->data->mass) / o->data->mass + c->data->mass;
						float masst2 = (2 * o->data->mass) / c->data->mass + o->data->mass;
					}
				}
				c = c->next;
			}
		}
		o = o->next;
	}


	printf("\n Delta Time : %f", dt);

	return true;
}
// Integration scheme: Velocity Verlet
// You should modularise all your algorithms into subroutines. Including the ones to compute forces.
void Physics::IntegratorVelocityVerlet(PhysObject* obj, float dt)
{
	obj->x += obj->v.x * dt + 0.5 * obj->a.x * dt * dt;
	obj->y += obj->v.y * dt + 0.5 * obj->a.y * dt * dt;
	obj->v.x += obj->a.x * dt;
	obj->v.y += obj->a.y * dt;
	if (obj->v.y >= 300 || obj->v.y <= -300)
	{
		obj->v.y = 0;
	}
	if (obj->v.x >= 300 || obj->v.x <= -300)
	{
		obj->v.x = 0;
	}
	printf("\nVel x: %f, y: %f", obj->v.x, obj->v.y);
}
void Physics::IntegratorVelocitySymplecticEuler(PhysObject* obj, float dt)
{
	       // Gravity will always act on the body
	obj->v.x += obj->a.x * dt;
	obj->v.y += obj->a.y * dt;
	obj->x += obj->v.x * dt;
	obj->y += obj->v.y * dt;
	if (obj->v.y >= 300 || obj->v.y <= -300)
	{
		obj->v.y = 0;
	}
	if (obj->v.x >= 300 || obj->v.x <= -300)
	{
		obj->v.x = 0;
	}
}
void Physics::IntegratorVelocityImplicitEuler(PhysObject* obj, float dt)
{
	obj->x += obj->v.x * dt;
	obj->y += obj->v.y * dt;
	obj->v.x += obj->a.x * dt;
	obj->v.y += obj->a.y * dt;
	if (obj->v.y >= 300 || obj->v.y <= -300)
	{
		obj->v.y = 0;
	}
	if (obj->v.x >= 300 || obj->v.x <= -300)
	{
		obj->v.x = 0;
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
	obj->setPendingToDelete = true;
}

