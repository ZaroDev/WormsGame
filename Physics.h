#include "p2List.h"


//Class: Ball
class PhysObject
{
public:
	PhysObject();
	~PhysObject();
	float Speed(float fx, float fy, float dt);

	// You could also use an array/vector
	float x;
	float y;

	// Velocity
	float vx;
	float vy;

	// Acceleration
	float ax;
	float ay;

	// Force (total) applied to the ball
	float fx;
	float fy;

	// Mass
	float mass;

	// Aerodynamics stuff
	float surface; // Effective wet surface
	float cl; // Lift coefficient
	float cd; // Drag coefficient

	// Has physics enabled?
	bool physics_enabled = true;
};
struct Atmosphere
{
	float density;
	float windx;
	float windy;
};
enum Integrator
{
	NONE = -1,
	VERLET,

};
class Physics
{
public:
	Physics();
	~Physics();

	bool Start(Integrator _integrator, float gx, float gy);
	
	bool Update(float dt);
	
	bool CleanUp();

	void CreateObject(PhysObject* obj);
	void IntegratorVelocityVerlet(PhysObject* ball, double dt);
	p2List<PhysObject*> objects;
private:
	float gravityX, gravityY;
	Atmosphere atmosphere;
	Integrator integrator = Integrator::VERLET;
};