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
	ball->x = 300.0f;
	ball->y = 0.0f;
	ball->mass = 50.0f;
	ball->shape = Shape::RECTANGLE;
	ball->w = 30;
	ball->h = 30;
	ball->density = .5f;
	ball->name.Create("Ball");
	ball->type = Type::DYNAMIC;
	ball->SetLimit(Vector2d(300.0f, 300.0f));


	ball2 = new PhysObject();
	ball2->x = 300.0f;
	ball2->y = 300.0f;
	ball2->mass = 1.0f;
	ball2->shape = Shape::RECTANGLE;
	ball2->w = 50;
	ball2->h = 30;
	ball2->restitution = 1.0f;
	ball2->name.Create( "Ground");
	ball2->type = Type::STATIC;

	App->physics->world.CreateObject(ball);
	App->physics->world.CreateObject(ball2);


	/*PhysObject* water = new PhysObject();
	water->x = 0;
	water->y = 500;
	water->shape = Shape::RECTANGLE;
	water->w = 2000;
	water->h = 250;
	water->density = 1.0f;
	water->mass = water->w * water->h * water->density;
	water->object = ObjectType::WATER;
	water->type = Type::STATIC;
	water->name.Create("Water");

	App->physics->world.CreateObject(water);
	App->physics->world.water = water;*/

	portal = new Portal();

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
		App->bulletDT = 1000 / 15;
	}
	if (App->input->GetKey(SDL_SCANCODE_1))
	{
		App->targetDT = 1000 / 30;
		App->bulletDT = 1000 / 30;
	}
	if (App->input->GetKey(SDL_SCANCODE_2))
	{
		App->targetDT = 1000 / 60;
		App->bulletDT = 1000 / 60;
	}
	if (App->input->GetKey(SDL_SCANCODE_3))
	{
		App->targetDT = 1000 / 144;
		App->bulletDT = 1000 / 144;
	}
	if (App->input->GetKey(SDL_SCANCODE_F2))
		App->controll = FrameTimeControll::VARIABLEDT;
	if (App->input->GetKey(SDL_SCANCODE_F3))
		App->controll = FrameTimeControll::FIXEDDTTM;
	if (App->input->GetKey(SDL_SCANCODE_F4))
		App->controll = FrameTimeControll::CONTROLLDT;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		PhysObject* o = new PhysObject(Shape::RECTANGLE, Type::DYNAMIC, (float)App->input->GetMouseX(), (float)App->input->GetMouseY(), 20, 20);
		o->name.Create("Box");
		o->SetLimit(Vector2d(300, 300));
		o->v.y = -10;
		App->physics->world.CreateObject(o);
	}

	string.Create("Current FPS: %f DeltaTime: %f  Expected FPS: %i, DeltaTime: %i", 1000 / App->dt, App->dt, 1000 / App->targetDT, App->targetDT);

	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
	{
		portal->SetPortal1((float)App->input->GetMouseX(), (float)App->input->GetMouseY(), 10, 10, Shape::RECTANGLE, Type::STATIC, PortalType::ORANGE, "Orange");
		if(!portal->active1)
			App->physics->world.CreateObject(portal->p1);

		App->physics->world.portal = portal;
	}
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		portal->SetPortal2((float)App->input->GetMouseX(), (float)App->input->GetMouseY(), 10, 10, Shape::RECTANGLE, Type::STATIC, PortalType::PURPLE, "Purple");
		if (!portal->active2)
			App->physics->world.CreateObject(portal->p2);
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		ball->cl = 10.0f;
	}



	App->window->SetTitle(string.GetString());

	return UPDATE_CONTINUE;
}
