#include "p2List.h"
#include "p2Point.h"
#include "SString.h"

enum class Shape
{
	NONE = -1,
	CIRCLE,
	RECTANGLE
};

enum class Type
{
	NONE = -1,
	DYNAMIC,
	KYNEMATIC,
	STATIC
};

//Class: Ball
class PhysObject
{
public:
	PhysObject();
	PhysObject(Shape shape_, Type type_, int x_, int y_, float w_, float h_ = 0);
	~PhysObject();

	
	void Reposition(float x, float y);

	float w = 0.0f;
	float h = 0.0f;
	float r = 0.0f;

	// You could also use an array/vector
	float x = 0.0f;
	float y = 0.0f;

	// Velocity
	float vx = 0.0f;
	float vy = 0.0f;

	// Acceleration
	float ax = 0.0f;
	float ay = 0.0f;

	// Force (total) applied to the ball
	float fx = 0.0f;
	float fy = 0.0f;

	// Mass
	float mass = 1.0f;

	// Aerodynamics stuff
	float surface = 0.2f; // Effective wet surface
	float cl = 0.12f; // Lift coefficient
	float cd = 0.04f; // Drag coefficient
	Shape shape;
	Type type;
	SString name;
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
	EULER

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
	void IntegratorVelocityVerlet(PhysObject* obj, float dt);
	void IntegratorVelocityEuler(PhysObject* obj, float dt);
	p2List<PhysObject*> objects;
	Atmosphere atmosphere;
	Integrator integrator = Integrator::EULER;
private:
	float gravityX, gravityY;
};