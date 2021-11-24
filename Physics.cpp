#include "Physics.h"
#include <math.h>

float Distance(int x1, int y1, int x2, int y2)
{
	return sqrtf(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)));
}
float DotProduct(float* a, float* b)
{
	float dot = 0;
	for (int i = 0; i < 2; i++)
		dot = dot +  a[i] * b[i];
	return dot;
}
void Rest(float* a, float* b, float* res)
{

	for (int i = 0; i < 2; i++) 
	{
		res[i] = a[i] - b[i];
	}

}
float Modulus(float* a)
{
	return sqrtf(powf(a[0], 2) + powf(a[1], 2));
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

bool PhysObject::Intersects(PhysObject* o)
{
	float normalx;
	float normaly;
	float dx, dy;

	bool ret = false;
	if (shape == Shape::RECTANGLE && o->shape == Shape::RECTANGLE)
	{
		ret = (x < o->x + o->w &&
			x + w > o->x &&
			y < o->y + o->h &&
			h + y > o->y);



	}

	if (shape == Shape::CIRCLE && o->shape == Shape::CIRCLE)
	{
		if (Distance(x, y, o->x, o->y) < (r + o->r)) ret = true;
	}

	if (shape == Shape::CIRCLE && o->shape == Shape::RECTANGLE)
	{
		p2Point<float> circleDistance;
		circleDistance.x = fabsf(x - o->x);
		circleDistance.y = fabsf(y - o->y);

		if (circleDistance.x > (o->w / 2 + r)) { return false; }
		if (circleDistance.y > (o->h / 2 + r)) { return false; }

		if (circleDistance.x <= (o->w / 2)) { return true; }
		if (circleDistance.y <= (o->h / 2)) { return true; }

		float cornerDistance_sq = powf((circleDistance.x - o->w / 2), 2) +
			powf((circleDistance.y - o->h / 2), 2);

		return (cornerDistance_sq <= (powf(r, 2)));
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
		if (o->data->type == Type::DYNAMIC)
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
			float fdx = -fdrag; // Let's assume Drag is aligned with x-axis (in your game, generalize this) Opuesta al vector speed = normalizar speed y multiplicar
			float fdy = flift; // Let's assume Lift is perpendicular with x-axis (in your game, generalize this)  Perpendicular al drag si la shape tiene lift
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
				if ( c->data->Intersects(o->data))
				{

					float v1[2] = { o->data->vx,  o->data->vy };
					

					float v2[2] = { c->data->vx,  c->data->vy };
					

					float x1[2] = { o->data->x,  o->data->y };
					

					float x2[2] = { c->data->x, c->data->y };
					p2Point<float> pv1;
					pv1.x = o->data->vx;
					pv1.y = o->data->vy;
					p2Point<float> pv2;
					pv2.x = c->data->vx;
					pv2.y = c->data->vy;
					p2Point<float> p1;
					p1.x = o->data->x;
					p1.y = o->data->y;
					p2Point<float> p2;
					p2.x = c->data->x;
					p2.y = c->data->y;

					float mass1 = (2 * c->data->mass) / (o->data->mass + c->data->mass);
					float rest1[2];
					Rest(v1, v2, rest1);
					float rest2[2];
					Rest(x1, x2, rest2);
					float dot = DotProduct(rest1, rest2);
					float mod = powf(Modulus(rest2), 2);
					float product1 = dot / mod;

					
					p2Point<float> mult1 = (p1 - p2) * (mass1 * product1);
					p2Point<float> v1P = (pv1 - mult1);


					float mass2 = (2 * o->data->mass) / (o->data->mass + c->data->mass);
					float rest3[2];
					Rest(v2, v1, rest3);
					float rest4[2];
					Rest(x2, x1, rest4);

					float product2 = DotProduct(rest1, rest2) / powf(Modulus(rest2), 2);
					p2Point<float> mult2 = (p2 - p1) * (mass2 * product2);
					p2Point<float> v2P = ( pv2 - mult2);

					o->data->vx = 0;
					o->data->vy = -41;

					c->data->vx = 0;
					c->data->vy = 41;

				

					printf("\n%s Object Expected vx: %f, vy: %f %s object vx: %f, vy: %f",o->data->name.GetString(), v1P.x, v1P.y,c->data->name.GetString(), v2P.x, v2P.y);
					printf("\n%s Object Actual vx: %f, vy: %f %s object vx: %f, vy: %f", o->data->name.GetString(), o->data->vx, o->data->vy, c->data->name.GetString(), c->data->vx, c->data->vy);
					//TODO COLLISION FORCES STUFF

					LOG("collision");
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
void Physics::IntegratorVelocityVerlet(PhysObject* obj, double dt)
{
	obj->x += obj->vx * dt + 0.5 * obj->ax * dt * dt;
	obj->y += obj->vy * dt + 0.5 * obj->ay * dt * dt;
	printf("\n%s vx: %f, vy: %f, ax: %f ay: %f", obj->name.GetString(), obj->vx, obj->vy, obj->ax, obj->ay);
	obj->vx += obj->ax * dt;
	obj->vy += obj->ay * dt;
	printf("\n%s updated vx: %f, vy: %f ax: %f ay: %f", obj->name.GetString(), obj->vx, obj->vy, obj->ax, obj->ay);
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

