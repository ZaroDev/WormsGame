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
	testFont = App->fonts->Load("Assets/Fonts/rtype_font.png", lookupTable, 1);

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
	fps.Create("Current FPS: %f DeltaTime: %f  Expected FPS: %i, DeltaTime: %i", 1000 / App->dt, App->dt, 1000 / App->targetDT, App->targetDT);

	App->fonts->BlitText(0, 10, testFont, fps.GetString());

}



