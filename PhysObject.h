#pragma once
#include "SString.h"
#include "Vector2d.h"

enum class Shape
{
	NONE = -1,
	CIRCLE,
	RECTANGLE
};
enum class ObjectType
{
	STANDART = -1,
	PORTAL,
	PLANT,
	SENSOR,
	WATER
};
enum class PortalType
{
	NONE = -1,
	ORANGE,
	PURPLE
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
	PhysObject(){}
	PhysObject(Shape shape_, Type type_, int x_, int y_, float w_, float h_);
	~PhysObject();
	


	float w = 0.0f;
	float h = 0.0f;
	float r = 0.0f;

	// You could also use an array/vector
	float x = 0.0f;
	float y = 0.0f;

	// Velocity
	Vector2d v;

	// Acceleration
	Vector2d a;

	// Force (total) applied to the ball
	Vector2d f;

	// Mass
	float mass = 1.0f;

	// Aerodynamics stuff
	float surface = .02f; // Effective wet surface
	float cl = 0.012f; // Lift coefficient
	float cd = 0.004f; // Drag coefficient

	float density = 1.0f;

	float restitution = 1.0f;
	Shape shape;
	Type type;
	SString name;
	ObjectType object = ObjectType::STANDART;
	PortalType portal = PortalType::NONE;
	bool setPendingToDelete = false;
	bool hasEnteredAPortal = false;
	bool hasLift = false;
	bool isOnWater = false;
	// Has physics enabled?
	bool physics_enabled = true;
	
};
