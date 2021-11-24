#include "ModuleUi.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"
#include "SString.h"

#include "p2Point.h"

#include <stdio.h>
#include <cstdio>

ModuleUi::ModuleUi(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	



}

ModuleUi::~ModuleUi()
{
	
}

bool ModuleUi::Start() {

	char lookupTable[] = { "! @,_./0123456789$;< ?abcdefghijklmnopqrstuvwxyz" };
	testFont = App->fonts->Load("Assets/Fonts/rtype_font3.png", lookupTable, 2);

	return true;
}

update_status ModuleUi::Update()
{
	if (App->physics->debug)
		Draw();


	return update_status::UPDATE_CONTINUE;
}






bool ModuleUi::CleanUp()
{


	return true;
}

void ModuleUi::Draw()
{
	SString fps;
	fps.Create("current fps %f deltatime %f ms expected fps %i deltatime %i ms", 1000 / App->dt, App->dt, 1000 / App->targetDT, App->targetDT);
	SString integrator;
	switch (App->physics->world.integrator)
	{
	case Integrator::VERLET:
		integrator.Create("current integrator verlet");
	break;
	case Integrator::IEULER:
		integrator.Create("current integrator implicit euler");
	break;
	case Integrator::SEULER:
		integrator.Create("current integrator symplectic euler");
	break;
	}

	SString wind;
	wind.Create("atmosphere density %f wind x %f wind y %f", App->physics->world.atmosphere.density, App->physics->world.atmosphere.windx, App->physics->world.atmosphere.windy);

	SString timeControll;
	switch (App->controll)
	{
	case FrameTimeControll::FIXEDDTTM :
		timeControll.Create("time controll fixed delta time");
		break;
	case FrameTimeControll::VARIABLEDT:
		timeControll.Create("time controll variable delta time");
		break;
	
	}

	App->fonts->BlitText(0, 10, testFont, fps.GetString());
	App->fonts->BlitText(0, 30, testFont, integrator.GetString());
	App->fonts->BlitText(0, 50, testFont, wind.GetString());
	App->fonts->BlitText(0, 70, testFont, timeControll.GetString());
}



