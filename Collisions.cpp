#include "Collisions.h"

#include "Application.h"

#include "ModuleRender.h"
#include "ModuleInput.h"
#include "SDL/include/SDL_Scancode.h"

Collisions::Collisions(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	/*matrix[Collider::Type::NONE][Collider::Type::NONE] = false;
	matrix[Collider::Type::NONE][Collider::Type::WALL] = false;
	matrix[Collider::Type::NONE][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::NONE][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::NONE][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::NONE][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::NONE][Collider::Type::WIN] = false;
	matrix[Collider::Type::NONE][Collider::Type::LVL2] = false;
	matrix[Collider::Type::NONE][Collider::Type::PU_AUTO] = false;
	matrix[Collider::Type::NONE][Collider::Type::PU_THREEWAY] = false;
	matrix[Collider::Type::NONE][Collider::Type::PU_POW] = false;

	matrix[Collider::Type::WALL][Collider::Type::NONE] = false;
	matrix[Collider::Type::WALL][Collider::Type::WALL] = false;
	matrix[Collider::Type::WALL][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::WALL][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::WALL][Collider::Type::PLAYER_SHOT] = true;
	matrix[Collider::Type::WALL][Collider::Type::ENEMY_SHOT] = true;
	matrix[Collider::Type::WALL][Collider::Type::WIN] = false;
	matrix[Collider::Type::WALL][Collider::Type::LVL2] = false;
	matrix[Collider::Type::WALL][Collider::Type::PU_AUTO] = false;
	matrix[Collider::Type::WALL][Collider::Type::PU_THREEWAY] = false;
	matrix[Collider::Type::WALL][Collider::Type::PU_POW] = false;

	matrix[Collider::Type::PLAYER][Collider::Type::NONE] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::WALL] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::PLAYER][Collider::Type::ENEMY_SHOT] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::WIN] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::LVL2] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PU_AUTO] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PU_THREEWAY] = true;
	matrix[Collider::Type::PLAYER][Collider::Type::PU_POW] = true;

	matrix[Collider::Type::ENEMY][Collider::Type::NONE] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::WALL] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::PLAYER_SHOT] = true;
	matrix[Collider::Type::ENEMY][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::PU_AUTO] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::PU_THREEWAY] = false;
	matrix[Collider::Type::ENEMY][Collider::Type::PU_POW] = false;

	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::NONE] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::WALL] = true;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::PLAYER] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::ENEMY] = true;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::PU_AUTO] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::PU_THREEWAY] = false;
	matrix[Collider::Type::PLAYER_SHOT][Collider::Type::PU_POW] = false;

	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::NONE] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::WALL] = true;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::PU_AUTO] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::PU_THREEWAY] = false;
	matrix[Collider::Type::ENEMY_SHOT][Collider::Type::PU_POW] = false;

	matrix[Collider::Type::PU_AUTO][Collider::Type::NONE] = false;
	matrix[Collider::Type::PU_AUTO][Collider::Type::PU_AUTO] = false;
	matrix[Collider::Type::PU_AUTO][Collider::Type::WALL] = false;
	matrix[Collider::Type::PU_AUTO][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::PU_AUTO][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::PU_AUTO][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::PU_AUTO][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::PU_AUTO][Collider::Type::PU_THREEWAY] = false;
	matrix[Collider::Type::PU_AUTO][Collider::Type::PU_POW] = false;

	matrix[Collider::Type::PU_THREEWAY][Collider::Type::NONE] = false;
	matrix[Collider::Type::PU_THREEWAY][Collider::Type::WALL] = false;
	matrix[Collider::Type::PU_THREEWAY][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::PU_THREEWAY][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::PU_THREEWAY][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::PU_THREEWAY][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::PU_THREEWAY][Collider::Type::PU_AUTO] = false;
	matrix[Collider::Type::PU_THREEWAY][Collider::Type::PU_POW] = false;

	matrix[Collider::Type::PU_POW][Collider::Type::NONE] = false;
	matrix[Collider::Type::PU_POW][Collider::Type::WALL] = false;
	matrix[Collider::Type::PU_POW][Collider::Type::PLAYER] = true;
	matrix[Collider::Type::PU_POW][Collider::Type::ENEMY] = false;
	matrix[Collider::Type::PU_POW][Collider::Type::PLAYER_SHOT] = false;
	matrix[Collider::Type::PU_POW][Collider::Type::ENEMY_SHOT] = false;
	matrix[Collider::Type::PU_POW][Collider::Type::PU_AUTO] = false;
	matrix[Collider::Type::PU_POW][Collider::Type::PU_THREEWAY] = false;*/
}

// Destructor
Collisions::~Collisions()
{

}

bool Collisions::Start()
{
	return true;
}

update_status Collisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (matrix[c1->type][c2->type] && c1->Intersects(c2->rect))
			{
				for (uint i = 0; i < MAX_LISTENERS; ++i)
					if (c1->listeners[i] != nullptr) c1->listeners[i]->OnCollision(c1, c2);

				for (uint i = 0; i < MAX_LISTENERS; ++i)
					if (c2->listeners[i] != nullptr) c2->listeners[i]->OnCollision(c2, c1);
			}
		}
	}

	return update_status::UPDATE_CONTINUE;
}

update_status Collisions::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	return update_status::UPDATE_CONTINUE;
}

update_status Collisions::PostUpdate()
{
	if (debug)
		DebugDraw();

	return update_status::UPDATE_CONTINUE;
}

void Collisions::DebugDraw()
{
	//Uint8 alpha = 80;
	//for (uint i = 0; i < MAX_COLLIDERS; ++i)
	//{
	//	if (colliders[i] == nullptr)
	//		continue;

	//	switch (colliders[i]->type)
	//	{
	//	case Collider::Type::NONE: // white
	//		App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
	//		break;
	//	case Collider::Type::WALL: // blue
	//		App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
	//		break;
	//	case Collider::Type::PLAYER: // green
	//		App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
	//		break;
	//	case Collider::Type::ENEMY: // red
	//		App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
	//		break;
	//	case Collider::Type::PLAYER_SHOT: // yellow
	//		App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
	//		break;
	//	case Collider::Type::ENEMY_SHOT: // magenta
	//		App->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha);
	//		break;
	//	case Collider::Type::WIN:
	//		App->render->DrawQuad(colliders[i]->rect, 255, 135, 0, alpha);
	//		break;
	//	case Collider::Type::LVL2:
	//		App->render->DrawQuad(colliders[i]->rect, 200, 255, 100, alpha);
	//		break;
	//	}
	//}
}

// Called before quitting
bool Collisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* Collisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
		}
	}

	return ret;
}
