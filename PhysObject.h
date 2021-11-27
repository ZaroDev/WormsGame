#pragma once
#include "SString.h"
#pragma once
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
	float surface = .02f; // Effective wet surface
	float cl = 0.012f; // Lift coefficient
	float cd = 0.004f; // Drag coefficient

	float density = 1.0f;

	Shape shape;
	Type type;
	SString name;
	ObjectType object = ObjectType::STANDART;
	PortalType portal = PortalType::NONE;
	int portalFrames = 0;
	bool hasEnteredAPortal = false;
	bool setPendingToDelete = false;
	bool hasLift = false;
	bool isOnWater = false;
	// Has physics enabled?
	bool physics_enabled = true;
	
};
