#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Module.h"
#include "Dummy.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
#include "ModuleFonts.h"
#include "ModuleUi.h"

enum class FrameTimeControll
{
	FIXEDDTTM = 0,
	VARIABLEDT,
	CONTROLLDT
};


class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModulePlayer* player;
	ModuleSceneIntro* scene_intro;
	ModulePhysics* physics;
	ModuleFonts* fonts;
	ModuleUi* UI;

private:

	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	uint targetDT = 1000 / 60;
	uint bulletDT = 1000 / 30;

	float dt;
	FrameTimeControll controll = FrameTimeControll::FIXEDDTTM;

private:

	void AddModule(Module* mod);
};