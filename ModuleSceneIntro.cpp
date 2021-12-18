#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "SString.h"
#include "Worm.h"
#include "EntityManager.h"

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

	PhysObject* g = new PhysObject();
	
	g->x = 150.0f;
	g->y = 750.0f;
	g->mass = 1.0f;
	g->shape = Shape::RECTANGLE;
	g->w = 5000;
	g->h = 50;
	g->restitution = 0.1f;
	g->name.Create("Ground1");
	g->type = Type::STATIC;

	App->physics->world.CreateObject(g);


	PhysObject* g2 = new PhysObject();

	g2->x = 500.0f;
	g2->y = 450.0f;
	g2->mass = 1.0f;
	g2->shape = Shape::RECTANGLE;
	g2->w = 100;
	g2->h = 50;
	g2->restitution = 0.1f;
	g2->name.Create("Ground2");
	g2->type = Type::STATIC;

	App->physics->world.CreateObject(g2);


	PhysObject* g3 = new PhysObject();

	g3->x = 100.0f;
	g3->y = 250.0f;
	g3->mass = 1.0f;
	g3->shape = Shape::RECTANGLE;
	g3->w = 210;
	g3->h = 50;
	g3->restitution = 0.1f;
	g3->name.Create("Ground2");
	g3->type = Type::STATIC;

	App->physics->world.CreateObject(g3);



	PhysObject* g4 = new PhysObject();

	g4->x = 800.0f;
	g4->y = 350.0f;
	g4->mass = 1.0f;
	g4->shape = Shape::RECTANGLE;
	g4->w = 200;
	g4->h = 50;
	g4->restitution = 0.1f;
	g4->name.Create("Ground2");
	g4->type = Type::STATIC;

	App->physics->world.CreateObject(g4);


	PhysObject* g5 = new PhysObject();

	g5->x = 650.0f;
	g5->y = 300.0f;
	g5->mass = 1.0f;
	g5->shape = Shape::RECTANGLE;
	g5->w = 100;
	g5->h = 50;
	g5->restitution = 0.1f;
	g5->name.Create("Ground2");
	g5->type = Type::STATIC;

	App->physics->world.CreateObject(g5);


	PhysObject* g6 = new PhysObject();

	g6->x = 250.0f;
	g6->y = 300.0f;
	g6->mass = 1.0f;
	g6->shape = Shape::RECTANGLE;
	g6->w = 100;
	g6->h = 50;
	g6->restitution = 0.1f;
	g6->name.Create("Ground2");
	g6->type = Type::STATIC;

	App->physics->world.CreateObject(g6);
	/*PhysObject* water = new PhysObject();
	water->x = 525;
	water->y = 600;
	water->shape = Shape::RECTANGLE;
	water->w = 1050;
	water->h = 250;
	water->density = 1.0f;
	water->mass = water->w * water->h * water->density;
	water->object = ObjectType::WATER;
	water->type = Type::STATIC;
	water->name.Create("Water");

	App->physics->world.CreateObject(water);
	App->physics->world.water = water;*/

	wormsRed.add((Worm*)App->entman->CreateEntity(EntityType::WORM, 50, 0, Team::RED));
	wormsRed.add((Worm*)App->entman->CreateEntity(EntityType::WORM, 150, 0, Team::RED));
	wormsRed.add((Worm*)App->entman->CreateEntity(EntityType::WORM, 250, 0, Team::RED));
	wormsRed.add((Worm*)App->entman->CreateEntity(EntityType::WORM, 350, 0, Team::RED));

	wormsBlue.add((Worm*)App->entman->CreateEntity(EntityType::WORM, 550, 0, Team::BLUE));
	wormsBlue.add((Worm*)App->entman->CreateEntity(EntityType::WORM, 650, 0, Team::BLUE));
	wormsBlue.add((Worm*)App->entman->CreateEntity(EntityType::WORM, 750, 0, Team::BLUE));
	wormsBlue.add((Worm*)App->entman->CreateEntity(EntityType::WORM, 850, 0, Team::BLUE));

	currentWormBlue = wormsBlue.getFirst();
	currentWormBlue->data->Select();
	currentWormRed = wormsRed.getFirst();
	blueTurn = true;
	redTurn = false;
	turnTimer = 1800;

	portal = new Portal();
	portalBlue = new Portal();
	portalRed = new Portal();

	background = App->textures->Load("Assets/Scene/back.png");
	return ret;
}

update_status ModuleSceneIntro::PreUpdate()
{
	for (p2List_item<Worm*>* w = wormsBlue.getFirst(); w != nullptr; w = w->next)
	{
		if (w->data->setPendingToDelete)

		{
			wormsBlue.del(w);
			break;
		}
	}
	for (p2List_item<Worm*>* w = wormsRed.getFirst(); w != nullptr; w = w->next)
	{
		if (w->data->setPendingToDelete)

		{
			wormsRed.del(w);
			break;
		}
	}


	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleSceneIntro::UpdateChoose()
{
	
	if (redTurn)
	{

		if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
		{
			currentWormRed->data->UnSelect();
			currentWormRed = currentWormRed->next;
			if (currentWormRed == nullptr)
				currentWormRed = wormsRed.getFirst();
			currentWormRed->data->Select();
		}
	}
	if (blueTurn)
	{

		if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
		{
			currentWormBlue->data->UnSelect();
			currentWormBlue = currentWormBlue->next;
			if (currentWormBlue == nullptr)
				currentWormBlue = wormsBlue.getFirst();
			currentWormBlue->data->Select();
		}
	}
}

void ModuleSceneIntro::IsStarting()
{
	if (App->input->GetKey(SDL_SCANCODE_W) || App->input->GetKey(SDL_SCANCODE_A) || App->input->GetKey(SDL_SCANCODE_D) || App->input->GetKey(SDL_SCANCODE_SPACE))
		turnStarted = true;
}



void ModuleSceneIntro::EndTurn()
{
	turnTimer = 1800;
	if (redTurn)
	{
		redTurn = false;
		blueTurn = true;
		turnStarted = false;
		currentWormBlue = wormsBlue.getFirst();
		currentWormBlue->data->Select();

		currentWormRed->data->UnSelect();
		return;
	}
	if (blueTurn)
	{
		redTurn = true;
		blueTurn = false;
		turnStarted = false;
		currentWormRed = wormsRed.getFirst();
		currentWormRed->data->Select();

		currentWormBlue->data->UnSelect();
		return;
	}
	
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	SString string;
	string.Create("Current FPS: %f DeltaTime: %f  Expected FPS: %i, DeltaTime: %i", 1000 / App->dt, App->dt, 1000 / App->targetDT, App->targetDT);
	if (currentWormBlue != nullptr)
	{
		weaponBlue = currentWormBlue->data->currentWeapon->data->name.GetString();
		ammoBlue = currentWormBlue->data->currentWeapon->data->ammo;

	}
	if (currentWormRed != nullptr)
	{
		weaponRed = currentWormRed->data->currentWeapon->data->name.GetString();
		ammoRed = currentWormRed->data->currentWeapon->data->ammo;
	}

	if (App->physics->debug)
	{
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

	}

	//if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	//{
	//	PhysObject* ball = new PhysObject();
	//	ball->x = App->input->GetMouseX();
	//	ball->y = App->input->GetMouseY();
	//	ball->mass = 1000.0f;
	//	ball->shape = Shape::RECTANGLE;
	//	ball->radius = 15;
	//	ball->w = 30;
	//	ball->h = 30;
	//	ball->restitution = 0.5f;
	//	ball->friction = 0.5f;
	//	ball->name.Create("Ball");
	//	ball->type = Type::DYNAMIC;
	//	ball->SetLimit(Vector2d(300.0f, 300.0f));
	//	
	//	App->physics->world.CreateObject(ball);
	//}
	if (!turnStarted)
	{
		UpdateChoose();
	}
	IsStarting();
	

	printf("\nTimer %i", turnTimer);
	if (turnStarted)
	{
		turnTimer--;
		if (turnTimer <= 0)
			EndTurn();
	}
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN && App->physics->debug)
		EndTurn();
	
	if (currentWormBlue->data->endTurn)
	{
		EndTurn();
		currentWormBlue->data->endTurn = false;
	}
	if (currentWormRed->data->endTurn)
	{
		EndTurn();
		currentWormRed->data->endTurn = false;
	}


	printf("\nRed turn %s", redTurn ? "true" : "false");
	printf("\nBlue turn %s", blueTurn ? "true" : "false");
	printf("\nWorms Red %i", wormsRed.count());
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

	



	App->renderer->Blit(background, 0, 0);

	App->window->SetTitle(string.GetString());

	return UPDATE_CONTINUE;
}
