#include <stdlib.h>
#include "Application.h"
#include "Globals.h"
#include <iostream>

#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

int main(int argc, char** argv)
{
	LOG("Starting game '%s'...", TITLE);

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;
	Application* App = NULL;
	
	uint startTime;
	float RealFPS;
	while (state != MAIN_EXIT)
	{
		startTime = SDL_GetTicks();
		switch (state)
		{
		case MAIN_CREATION:

			LOG("-------------- Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG("-------------- Application Init --------------");
			if (App->Init() == false)
			{
				LOG("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG("-------------- Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				LOG("Application Update exits with ERROR");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
		break;

		case MAIN_FINISH:

			LOG("-------------- Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG("Application CleanUp exits with ERROR");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
		switch (App->controll)
		{
		case FrameTimeControll::FIXEDDTTM :
		{
			if (App->targetDT > SDL_GetTicks() - startTime)
			{
				SDL_Delay(App->targetDT - (SDL_GetTicks() - startTime));
			}
			RealFPS = (SDL_GetTicks() - startTime);
			App->dt = RealFPS;
			break;
		}
		case FrameTimeControll::VARIABLEDT :
		{
			RealFPS = (SDL_GetTicks() - startTime);
			App->dt = RealFPS;
			break;
		}
		case FrameTimeControll::CONTROLLDT :
		{
			if (App->targetDT > SDL_GetTicks() - startTime)
			{
				SDL_Delay(App->targetDT - (SDL_GetTicks() - startTime));
			}
			App->dt = App->bulletDT;
		}
		}

		
		
		

	}

	delete App;
	LOG("Exiting game '%s'...\n", TITLE);
	return main_return;
}