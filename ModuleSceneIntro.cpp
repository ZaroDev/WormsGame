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
	ball->mass = 1.0f;
	ball->shape = Shape::RECTANGLE;
	ball->w = 30;
	ball->h = 30;
	ball->r = 30;
	ball->name.Create("Ball");
	ball->type = Type::DYNAMIC;


	ball2 = new PhysObject();
	ball2->x = 300.0f;
	ball2->y = 300.0f;
	ball2->mass = 3.0f;
	ball2->shape = Shape::RECTANGLE;
	ball2->w = 30;
	ball2->h = 30;
	ball2->r = 30;
	ball2->name.Create( "Ground");
	ball2->type = Type::STATIC;

	App->physics->world.CreateObject(ball);
	App->physics->world.CreateObject(ball2);

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
		App->physics->world.CreateObject(o);
	}

	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		Portal* p;
		if (portals.Pop(p))
		{
			p->SetPortal1((float)App->input->GetMouseX(), (float)App->input->GetMouseY(), 20, 20, Shape::CIRCLE, Type::STATIC, PortalType::ORANGE, "Orange");
			App->physics->world.CreateObject(p->p1);
		}
		else
		{
			//Si no hay un portal con un miembro vacio lo creo y lo añado
			p = new Portal;
			p->SetPortal1((float)App->input->GetMouseX(), (float)App->input->GetMouseY(), 20, 20, Shape::CIRCLE, Type::STATIC, PortalType::ORANGE, "Orange");
			App->physics->world.CreateObject(p->p1);
			portals.Push(p);
		}
		App->physics->world.portal = p;
		
	}
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
	{
		Portal* p;
		if (portals.Pop(p))
		{
			p->SetPortal2((float)App->input->GetMouseX(), (float)App->input->GetMouseY(), 20, 20, Shape::CIRCLE, Type::STATIC, PortalType::PURPLE, "Purple");
			App->physics->world.CreateObject(p->p2);
		}
		else
		{
			//Si no hay un portal con un miembro vacio lo creo y lo añado
			p = new Portal;
			p->SetPortal2((float)App->input->GetMouseX(), (float)App->input->GetMouseY(), 20, 20, Shape::CIRCLE, Type::STATIC, PortalType::PURPLE, "Purple");
			App->physics->world.CreateObject(p->p2);
			portals.Push(p);
		}
		
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		App->physics->world.DestroyObject(ball);
	}
	string.Create("Current FPS: %f DeltaTime: %f  Expected FPS: %i, DeltaTime: %i", 1000 / App->dt, App->dt, 1000 / App->targetDT, App->targetDT);


	App->window->SetTitle(string.GetString());

	return UPDATE_CONTINUE;
}
