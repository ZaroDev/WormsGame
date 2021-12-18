#pragma once
#include "Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "ModulePhysics.h"
#include "Portal.h"

class Entity;
class Worm;
class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	void UpdateChoose();
	void IsStarting();
	void StartTurn();
	void EndTurn();
public:
	PhysObject* ball;
	PhysObject* ball2;
	Portal* portalRed;
	Portal* portalBlue;
	//Current portal
	Portal* portal;

	p2List<Worm*> wormsRed;
	p2List<Worm*> wormsBlue;
	p2List_item<Worm*>* currentWormRed;
	p2List_item<Worm*>* currentWormBlue;

	int turnTimer;
	bool startTurn;
	bool redTurn;
	bool blueTurn;
	bool turnStarted;
private:

	SDL_Texture* background;
	SDL_Texture* floor;
};
