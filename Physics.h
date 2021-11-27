#include "p2List.h"
#include "p2Point.h"
#include "SString.h"
#include "PhysObject.h"
#include "Portal.h"

struct Atmosphere
{
	float density;
	float windx;
	float windy;
};
enum class Integrator
{
	NONE = -1,
	VERLET,
	SEULER,
	IEULER

};
class Physics
{
public:
	Physics();
	~Physics();

	bool Start(Integrator _integrator, float gx, float gy);
	bool PreUpdate();
	bool Update(float dt);

	bool CleanUp();

	void CreateObject(PhysObject* obj);
	void DestroyObject(PhysObject* obj);
	void IntegratorVelocityVerlet(PhysObject* obj, float dt);
	void IntegratorVelocitySymplecticEuler(PhysObject* obj, float dt);
	void IntegratorVelocityImplicitEuler(PhysObject* obj, float dt);
	Portal* portal;
	PhysObject* water;
	Atmosphere atmosphere;
	Integrator integrator = Integrator::IEULER;
	p2List<PhysObject*> objects;
private:
	float gravityX, gravityY;
};