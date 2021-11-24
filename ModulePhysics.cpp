#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include "p2List.h"

// TODO 1: Include Box 2 header and library

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world.Start(Integrator::VERLET, 0.0f, 1.0f);
	world.atmosphere.density = 1.0f;
	world.atmosphere.windx = 0.0f;
	world.atmosphere.windy = 0.0f;
	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	// TODO 3: Update the simulation ("step" the world)
	world.Update(App->dt / 100);
	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	p2List_item<PhysObject*>* o = world.objects.getFirst();

	while (o != NULL)
	{
		switch (o->data->shape)
		{
		case Shape::CIRCLE:
			App->renderer->DrawCircle(o->data->x, o->data->y, o->data->r, 255, 0, 0, 255);
			break;
		case Shape::RECTANGLE:
			SDL_Rect rect = { o->data->x - o->data->w / 2, o->data->y - o->data->h / 2, o->data->w,o->data->h };
			App->renderer->DrawQuad(rect, 0, 255, 0, 255, false);
			break;
		}
		printf("\nName: %s", o->data->name.GetString());
		printf("\nx: %f, y: %f", o->data->x, o->data->y);
		printf("\nvx: %f, vy: %f", o->data->vx, o->data->vy);		
		printf("\nfx: %f, fy: %f", o->data->fx, o->data->fy);


		o = o->next;
	}
	

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!

	return true;
}
