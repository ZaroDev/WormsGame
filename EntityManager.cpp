#include "EntityManager.h"
#include "Physics.h"
#include "Application.h"
#include "Worm.h"

#include "ModuleFonts.h"


EntityManager::EntityManager(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	
}

EntityManager::~EntityManager()
{
	CleanUp();
}


bool EntityManager::Start()
{
	char lookupTable[] = { "! @,_./0123456789$;< ?abcdefghijklmnopqrstuvwxyz" };
	testFont = App->fonts->Load("Assets/Fonts/rtype_font3.png", lookupTable, 2);
	return true;
}

update_status EntityManager::PreUpdate()
{
	for (p2List_item<Entity*>* ent = entities.getFirst(); ent != NULL; ent = ent->next)
	{
		if (ent->data->setPendingToDelete) DestroyEntity(ent->data);
	}

	return UPDATE_CONTINUE;
}
void EntityManager::UpdateAll(float dt, bool doLogic)
{
	if (!doLogic)
		return;

	for (p2List_item<Entity*>* ent = entities.getFirst(); ent != NULL; ent = ent->next)
	{
		ent->data->Update(dt);
		ent->data->Draw();
		if (ent->data->type == EntityType::WORM)
		{
			SString tmp("%s %i", ent->data->name.GetString(), ent->data->health);
			App->fonts->BlitText(ent->data->GetPos().x - 20, ent->data->GetPos().y - 20, testFont, tmp.GetString());
		}
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


Entity* EntityManager::CreateEntity(EntityType type, float x, float y, Team team)
{
	Entity* ret = nullptr;
	p2Point<float> pos;
	pos.x = x;
	pos.y = y;
	switch (type)
	{
		case EntityType::WORM:
		{
			ret = new Worm(pos, team);
			App->physics->world.CreateObject(ret->pbody);
			break;
		}
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
	for (p2List_item<Entity*>* ent = entities.getFirst(); ent != NULL; ent = ent->next)
	{
		DestroyEntity(ent->data);
	}
}