#include "ModuleUi.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleFonts.h"
#include "SString.h"
#include "ModulePlayer.h"
#include "p2Point.h"

#include <stdio.h>
#include <cstdio>

ModuleUi::ModuleUi(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	score = 0;
	prevScore = -1;
	highScore = -1;



}

ModuleUi::~ModuleUi()
{
	
}

bool ModuleUi::Start() {

	App->fonts->Load("pinball/Sprites/numbers_clear.png", "0123456789-", 1);
	App->fonts->Load("pinball/Sprites/numbers_orange.png", "0123456789-", 1);
	App->fonts->Load("pinball/Sprites/numbers_pink.png", "0123456789-", 1);
	App->fonts->Load("pinball/Sprites/numbers_purple.png", "0123456789-", 1);
	App->fonts->Load("pinball/Sprites/numbers_end.png", "0123456789", 1);

	return true;
}

update_status ModuleUi::PreUpdate()
{
	return update_status::UPDATE_CONTINUE;
}




bool ModuleUi::CleanUp()
{


	return true;
}

void ModuleUi::Draw()
{
	/*App->renderer->Blit(uiTex, 0, 0, NULL);
	p2Point<int> pos = { (512 / 2) - (7 * 10) ,46 };

	 DRAW SCORE
	if (score >= 999999999)
	{
		App->fonts->BlitText(pos.x, pos.y, 0, "9999999");
	}
	else if (score < 0) App->fonts->BlitText(pos.x, pos.y, 0, "-------");
	else
	{
		SString text("%7d", score);
		App->fonts->BlitText(pos.x, pos.y, 0, text.GetString());
	}

	 DRAW MAX SCORE
	pos = { 0,72 };
	if (highScore >= 999999999)
	{
		App->fonts->BlitText(pos.x, pos.y, 1, "9999999");
	}
	else if (highScore < 0) App->fonts->BlitText(pos.x, pos.y, 1, "-------");
	else
	{
		SString text("%7d", highScore);
		App->fonts->BlitText(pos.x, pos.y, 1, text.GetString());
	}

	 DRAW PREV SCORE
	pos = { 512 - (7 * 20),72 };
	if (prevScore >= 999999999)
	{
		App->fonts->BlitText(pos.x, pos.y, 2, "9999999");
	}
	else if (prevScore < 0) App->fonts->BlitText(pos.x, pos.y, 1, "-------");
	else
	{
		SString text("%7d", prevScore);
		App->fonts->BlitText(pos.x, pos.y, 2, text.GetString());
	}

	pos = {448, 854};
	SString text("%1d", App->player->lives);
	App->fonts->BlitText(pos.x, pos.y, 3, text.GetString());*/

}


void ModuleUi::AddScore(int value)
{
	score += value;
	


}

void ModuleUi::UpdateScores()
{
	prevScore = score;
	if (score > highScore)highScore = score;
	score = 0;

}

void ModuleUi::DrawHighScore()
{
	SString text("%7d", highScore);
	if (highScore < 0)text = "0000000";
	if (highScore > 9999999) text = "9999999";
	App->fonts->BlitText(32, 250, 4, text.GetString());
}
