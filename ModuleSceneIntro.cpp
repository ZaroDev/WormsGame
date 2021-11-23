#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "SString.h"



ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;
	ball = new PhysObject();
	ball->x = 350.0f;
	ball->y = 300.0f;
	ball->mass = 1.0f;


	App->physics->world.CreateObject(ball);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	SString string;
	string.Create("FPS: %f DeltaTime: %f", 1000 / App->dt, App->dt);
	App->window->SetTitle(string.GetString());

	return UPDATE_CONTINUE;
}
