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

	char lookupTable[] = { "!^#$%&'()*+, ./0123456789:;<=>?€abcdefghijklmnopqrstuvwxyz[\]^-'abcdefghijklmnopqrstuvwxyz{|}_" };
	testFont = App->fonts->Load("Assets/Fonts/ddrtiny.bmp", lookupTable, 1);

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
	fps.Create("current fps %f deltatime %f ms  expected fps %i deltatime %i ms", 1000 / App->dt, App->dt, 1000 / App->targetDT, App->targetDT);
	SString integrator;
	switch (App->physics->world.integrator)
	{
	case Integrator::VERLET:
		integrator.Create("current integrator verlet");
		
	break;
	}

	SString wind;
	wind.Create("atmosphere density %f wind x %f wind y %f", App->physics->world.atmosphere.density, App->physics->world.atmosphere.windx, App->physics->world.atmosphere.windy);


	App->fonts->BlitText(0, 10, testFont, fps.GetString());
	App->fonts->BlitText(0, 30, testFont, integrator.GetString());
	App->fonts->BlitText(0, 50, testFont, wind.GetString());

}



