#ifndef __ENMAN_H__
#define __ENMAN_H__

#include "Entity.h"
#include "Module.h"
#include "p2List.h"

class EntityManager : public Module
{
public:
	EntityManager(Application* app, bool startEnabled);
	virtual ~EntityManager();
	//Module Functions
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void OnCollision(PhysObject* bodyA, PhysObject* bodyB);
	
	//Updates all the entities
	void UpdateAll(float dt, bool doLogic);
	//Util functions
	Entity* CreateEntity(EntityType type_, p2Point<float> position_);
	void DestroyAllEntities();
	void DestroyEntity(Entity* entity);

private:
	p2List<Entity*> entities;
	float accumulatedTime;
	float updateMsCycle;
	bool doLogic;

};
#endif