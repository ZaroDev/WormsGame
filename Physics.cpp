#include "Physics.h"
#include <math.h>
# define M_PI           3.1415

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
		if (o->b < c->t || o->t > c->b || o->l > c->r || o->r < c->l)
			ret = false;
		else
			ret = true;
	}

	if (o->shape == Shape::CIRCLE && c->shape == Shape::CIRCLE)
	{
		if (Distance(o->x, o->y, c->x, c->y) < (o->radius + c->radius)) ret = true;
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
	printf("\n%i objects", objects.count());

	for (p2List_item<PhysObject*>* o = objects.getFirst(); o != NULL; o = o->next)
	{
		if (o->data->physics_enabled)
		{
			switch (o->data->type)
			{
			case Type::DYNAMIC:
			{
				// Step #0: Reset total acceleration and total accumulated force of the ball (clear old values)
				o->data->f.x = o->data->f.y = o->data->fp.x = o->data->fp.y = 0.0;
				o->data->a.x = o->data->a.y = 0.0;

				// Step #1: Compute forces
					// Compute Gravity force
				float fgx = o->data->mass * gravityX;
				float fgy = o->data->mass * gravityY; // Let's assume gravity is constant and downwards

				// Add gravity force to the total accumulated force of the ball
				o->data->f.x += fgx;
				o->data->f.y += fgy;

				// Compute Aerodynamic Lift & Drag forces
				float speed =Vector2d::Magnitude( o->data->v - atmosphere.wind);
				if (o->data->v.x != 0 && !o->data->isOnWater)
				{
					float fdrag = 0.5 * atmosphere.density * speed * speed * o->data->surface * o->data->cd;
					float fdx = -fdrag; // Let's assume Drag is aligned with x-axis (in your game, generalize this) Opuesta al vector speed = normalizar speed y multiplicar
					o->data->f.x += fdx;
				}

				// Let's assume Lift is perpendicular with x-axis (in your game, generalize this)  Perpendicular al drag si la shape tiene lift
				if (o->data->v.y != 0 && !o->data->isOnWater)
				{
					float flift = 0.5 * atmosphere.density * speed * speed * o->data->surface * o->data->cl;
					float fdy = -flift;
					o->data->f.y += fdy;
				}


				//Hydrodinamic Forces
				if (o->data->isOnWater)
				{

					//Fd = -b * v;
					float hDragX = 6 * M_PI * o->data->w / 2 * o->data->v.x;
					float hDragY = 6 * M_PI * o->data->h / 2 * o->data->v.y;
					


					//Fb = p g V;
					float bouyancyX =  gravityX * (water->density / o->data->density) * o->data->h * o->data->w;
					float bouyancyY =  gravityY * (water->density / o->data->density) * o->data->h * o->data->w;
					//Need to work on it
					float bfx = (-bouyancyX + hDragX) *(-1);
					float bfy = (-bouyancyY + hDragY) *(-1);
					
					o->data->f.x += bfx;
					o->data->f.y += bfy;
					o->data->isOnWater = false;
				}
				//Player Input forces (Doesn't work)
				o->data->f.x += o->data->fp.x;
				o->data->f.y += o->data->fp.y;
				// Step #2: 2nd Newton's Law: SUM_Forces = mass * accel --> accel = SUM_Forces / mass
				o->data->a.x = o->data->f.x / o->data->mass;
				o->data->a.y = o->data->f.y / o->data->mass;
				/*printf("\nAcc x: %f, y: %f", o->data->a.x, o->data->a.y);
				printf("\nVel x: %f, y: %f", o->data->v.x, o->data->v.y);*/
			}
			}
			//The old position boundaries updates
			o->data->ol = o->data->x - o->data->w / 2;
			o->data->oR = o->data->x + o->data->w / 2;
			o->data->ot = o->data->y - o->data->h / 2;
			o->data->ob = o->data->y + o->data->h / 2;

			/*printf("\nfx: %f, fy: %f", o->data->f.x, o->data->f.y);*/
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
			//The new position boundaries gets updated after the integration
			o->data->l = o->data->x - o->data->w / 2;
			o->data->r = o->data->x + o->data->w / 2;
			o->data->t = o->data->y - o->data->h / 2;
			o->data->b = o->data->y + o->data->h / 2;
				
			
			

			//Collision Solver
			for(p2List_item<PhysObject*>* c = objects.getFirst(); c != NULL; c = c->next)
			{
				if (c->data != o->data)
				{
					if (Intersects(o->data, c->data))
					{
						/*printf("\ncollision 1: %s 2: %s", o->data->name.GetString(), c->data->name.GetString());*/

						//Else if statement We don't collide with more that one type per object
						if (o->data != water && c->data == water)
						{
							o->data->isOnWater = true;
							break;
						}
						else if (o->data->object == ObjectType::PORTAL && c->data->object != ObjectType::PORTAL)
						{
							portal->Teletransport(o->data, c->data);
							break;
						}
						else if (c->data->type == Type::STATIC)
						{
							ComputeOverlaping(o->data, c->data);
							break;
						}
						else if(c->data->type == Type::DYNAMIC && o->data->type == Type::DYNAMIC)
						{
							ComputeElasticCollision(o->data, c->data);
							break;
						}
					}

				}
			}
		}
	}


	printf("\n Delta Time : %f", dt);

	return true;
}
// Integration scheme: Velocity Verlet
// You should modularise all your algorithms into subroutines. Including the ones to compute forces.
void Physics::IntegratorVelocityVerlet(PhysObject* obj, float dt)
{
	//Verlet Integrator
	obj->x += obj->v.x * dt + 0.5 * obj->a.x * dt * dt;
	obj->y += obj->v.y * dt + 0.5 * obj->a.y * dt * dt;
	obj->v.x += obj->a.x * dt;
	obj->v.y += obj->a.y * dt;
	//Speed limiter
	if (obj->v.x > obj->limitSpeed.x) obj->v.x = obj->limitSpeed.x;
	if (obj->v.x < -obj->limitSpeed.x) obj->v.x = -obj->limitSpeed.x;
	if (obj->v.y > obj->limitSpeed.y) obj->v.y = obj->limitSpeed.y;
	if (obj->v.y < -obj->limitSpeed.y)obj->v.y = -obj->limitSpeed.y;

}
void Physics::IntegratorVelocitySymplecticEuler(PhysObject* obj, float dt)
{
	//Sympletic Euler Integrator
	obj->v.x += obj->a.x * dt;
	obj->v.y += obj->a.y * dt;
	obj->x += obj->v.x * dt;
	obj->y += obj->v.y * dt;
	//Speed limiter
	if (obj->v.x > obj->limitSpeed.x) obj->v.x = obj->limitSpeed.x;
	if (obj->v.x < -obj->limitSpeed.x) obj->v.x = -obj->limitSpeed.x;
	if (obj->v.y > obj->limitSpeed.y) obj->v.y = obj->limitSpeed.y;
	if (obj->v.y < -obj->limitSpeed.y)obj->v.y = -obj->limitSpeed.y;
}
void Physics::IntegratorVelocityImplicitEuler(PhysObject* obj, float dt)
{
	//Implicit Euler Integrator
	obj->x += obj->v.x * dt;
	obj->y += obj->v.y * dt;
	obj->v.x += obj->a.x * dt;
	obj->v.y += obj->a.y * dt;
	//Speed limiter
	if (obj->v.x > obj->limitSpeed.x) obj->v.x = obj->limitSpeed.x;
	if (obj->v.x < -obj->limitSpeed.x) obj->v.x = -obj->limitSpeed.x;
	if (obj->v.y > obj->limitSpeed.y) obj->v.y = obj->limitSpeed.y;
	if (obj->v.y < -obj->limitSpeed.y)obj->v.y = -obj->limitSpeed.y;
}
bool Physics::CleanUp()
{
	bool ret = true;
	objects.clear();
	return true;
}

void Physics::ComputeElasticCollision(PhysObject* o, PhysObject* c)
{
	//POSITION SOLVING TODO
	if (o->shape == Shape::CIRCLE && c->shape == Shape::CIRCLE)
	{
		//VELOCITY SOLVING
		//2m2/m1+m2
		float mass1 = (2.0f * c->mass) / (o->mass + c->mass);
		float mass2 = (2.0f * o->mass) / (o->mass + c->mass);

		//dot(v1-v2, x1-x2) / ||x1-x2||^2
		Vector2d x1;
		x1.x = o->x;
		x1.y = o->y;
		Vector2d x2;
		x2.x = c->x;
		x2.y = c->y;
		Vector2d x1_x2 = (x1 - x2);
		Vector2d x2_x1 = (x2 - x1);
		float dot1 = Vector2d::DotProduct(o->v - c->v, x1_x2) / pow(Vector2d::Magnitude(x1_x2), 2);
		float dot2 = Vector2d::DotProduct(c->v - o->v, x2_x1) / pow(Vector2d::Magnitude(x2_x1), 2);

		// Compute velocities after collision (assume perfectly elastic collision without dampening)
		o->v = o->v - (x1_x2 * mass1 * dot1);
		c->v = c->v - (x2_x1 * mass2 * dot2);

		// Apply restitution coefficient (FUYM inelasticity/dampening)
		o->v = o->v * o->restitution;
		c->v = c->v * c->restitution;
		
		//https://flatredball.com/documentation/tutorials/math/circle-collision/

		//Position solving
		float angle = atan2f(c->y - o->y, c->x - o->x);
		float distanceBetweenCircles = sqrtf((c->x - o->x) * (c->x - o->x) + (c->y - o->y) * (c->y - o->y));
		float sumOfRadius = o->radius + c->radius;
		float distanceToMove = sumOfRadius - distanceBetweenCircles;
		c->x += cosf(angle) * distanceToMove;
		c->y += sinf(angle) * distanceToMove;
	}
	else if (o->shape == Shape::RECTANGLE && c->shape == Shape::RECTANGLE)
	{
		//https://www.youtube.com/watch?v=8JJ-4JgR7Dg need to watch
		if (o->b >= c->t && o->ob < c->ot)
		{
			o->y = c->t - (o->h / 2) - 0.1f;
			o->v.y = -o->v.y * o->restitution;
			o->v.x = o->v.x * o->friction;
			c->v.y = -c->v.y * o->restitution;
			c->v.x = c->v.x * o->friction;
		}
		else if (o->t <= c->b && o->ot > c->ob)
		{
			o->y = c->b + (o->h / 2) + 0.1f;
			o->v.y = -o->v.y * o->restitution;
			o->v.x = o->v.x * o->friction;
			c->v.y = -c->v.y * o->restitution;
			c->v.x = c->v.x * o->friction;
		}
		else if (o->r >= c->l && o->oR < c->ol)
		{
			o->x = c->l - (o->w / 2) - 0.1f;
			o->v.y = -o->v.y * o->restitution;
			o->v.x = o->v.x * o->friction;
			c->v.y = -c->v.y * o->restitution;
			c->v.x = c->v.x * o->friction;

		}
		else if (o->l <= c->r && o->ol > c->oR)
		{
			o->x = c->r + (o->w / 2) + 0.2f;
			o->v.y = -o->v.y * o->restitution;
			o->v.x = o->v.x * o->friction;
			c->v.y = -c->v.y * o->restitution;
			c->v.x = c->v.x * o->friction;
		}
	}
	else if (o->shape == Shape::RECTANGLE && c->shape == Shape::CIRCLE)
	{
		//https://www.youtube.com/watch?v=D2a5fHX-Qrs need to watch
	}
	else if (o->shape == Shape::CIRCLE && c->shape == Shape::RECTANGLE)
	{

	}
}

void Physics::ComputeOverlaping(PhysObject* o, PhysObject* c)
{
	if (o->b >= c->t && o->ob < c->ot)
	{
		o->y = c->t - (o->h / 2) - 0.1f;
		o->v.y = -o->v.y * o->restitution;
		o->v.x = o->v.x * o->friction;
	}
	else if (o->t <= c->b && o->ot > c->ob)
	{
		o->y = c->b + (o->h / 2) + 0.1f;
		o->v.y = -o->v.y * o->restitution;
		o->v.x = o->v.x * o->friction;
	}
	else if (o->r >= c->l && o->oR < c->ol)
	{
		o->x = c->l - (o->w / 2) - 0.1f;
		o->v.y = -o->v.y * o->restitution;
		o->v.x = o->v.x * o->friction;

	}
	else if (o->l <= c->r && o->ol > c->oR)
	{
		o->x = c->r + (o->w / 2) + 0.2f;
		o->v.y = -o->v.y * o->restitution;
		o->v.x = o->v.x * o->friction;
	}
}

void Physics::CreateObject(PhysObject* obj)
{
	objects.add(obj);
}

void Physics::DestroyObject(PhysObject* obj)
{
	obj->setPendingToDelete = true;
}