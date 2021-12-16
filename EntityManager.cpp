#include "EntityManager.h"
#include "Physics.h"
#include "Application.h"





EntityManager::EntityManager(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	
}

EntityManager::~EntityManager()
{
	CleanUp();
}


bool EntityManager::Start()
{
	return true;
}

update_status EntityManager::PreUpdate()
{
	for (p2List_item<Entity*>* ent = entities.getFirst(); ent != nullptr; ent = ent->next)
	{
		if (ent->data->setPendingToDelete) DestroyEntity(ent->data);
	}

	return UPDATE_CONTINUE;
}
void EntityManager::UpdateAll(float dt, bool doLogic)
{
	if (!doLogic)
		return;

	for (p2List_item<Entity*>* ent = entities.getFirst(); ent != nullptr; ent = ent->next)
	{
		ent->data->Update(dt);
	}
	return;
}
update_status EntityManager::Update()
{
	accumulatedTime += App->dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;
	UpdateAll(App->dt, doLogic);
	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}
	return UPDATE_CONTINUE;

}

update_status EntityManager::PostUpdate()
{
	for (p2List_item<Entity*>* ent = entities.getFirst(); ent != nullptr; ent = ent->next)
	{
		ent->data->Draw();
	}
	return UPDATE_CONTINUE;
}

bool EntityManager::CleanUp()
{
	DestroyAllEntities();
	return true;
}

void EntityManager::OnCollision(PhysObject* bodyA, PhysObject* bodyB)
{
}


Entity* EntityManager::CreateEntity(EntityType type, p2Point<float> position_)
{
	Entity* ret = nullptr;
	switch (type)
	{
		/*case EntityType::PLAYER: ret = new Player(position); break;
		case EntityType::ENEMY_EAGLE: ret = new Eagle(position); break;
		case EntityType::ENEMY_RAT: ret = new Rat(position); break;
		case EntityType::GEM: ret = new Gem(position); break;
		case EntityType::CHERRY	: ret = new Cherry(position); break;
		case EntityType::CHECKPOINT: ret = new CheckPoint(position); break;*/
	}
	if (ret != nullptr) entities.add(ret);
	return ret;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	App->physics->world.DestroyObject(entity->pbody);
	entities.del(entities.findNode(entity));
}
void EntityManager::DestroyAllEntities()
{
	for (p2List_item<Entity*>* ent = entities.getFirst(); ent != nullptr; ent = ent->next)
	{
		DestroyEntity(ent->data);
	}
}