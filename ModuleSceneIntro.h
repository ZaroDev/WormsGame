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
	update_status Update();
	bool CleanUp();

public:
	PhysObject* ball;
	PhysObject* ball2;
	Portal* portal;

	p2List<Worm*> worms;
	p2List_item<Worm*>* currentWorm;
};
