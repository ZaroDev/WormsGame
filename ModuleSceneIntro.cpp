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
	ball->y = 0.0f;
	ball->mass = 1.0f;
	ball->shape = Shape::CIRCLE;
	ball->r = 10;

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
	
	if (App->input->GetKey(SDL_SCANCODE_4))
	{
		App->targetDT = 1000 / 15;
	}
	if (App->input->GetKey(SDL_SCANCODE_1))
	{
		App->targetDT = 1000 / 30;
	}
	if (App->input->GetKey(SDL_SCANCODE_2))
	{
		App->targetDT = 1000 / 60;
	}
	if (App->input->GetKey(SDL_SCANCODE_3))
	{
		App->targetDT = 1000 / 144;
	}
	


	
	
	string.Create("Current FPS: %f DeltaTime: %f  Expected FPS: %i, DeltaTime: %i", 1000 / App->dt, App->dt, 1000 / App->targetDT, App->targetDT);


	App->window->SetTitle(string.GetString());

	return UPDATE_CONTINUE;
}
