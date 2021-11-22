#pragma once
#include "Module.h"
#include "Globals.h"
#include "Physics.h"
class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	Physics world;
private:
	bool debug;
};