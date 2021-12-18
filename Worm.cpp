#include "Worm.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "Application.h"
#include "ModuleInput.h"
#include "Animation.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

Worm::Worm(p2Point<float> position_, Team team_, Application* app_) : Entity(EntityType::WORM, position_, team_, app_)
{
	name.Create("worm");
	pbody = new PhysObject();
	pbody->mass = 1000.0f;
	pbody->x = position.x;
	pbody->y = position.y;
	pbody->type = Type::DYNAMIC;
	pbody->shape = Shape::RECTANGLE;
	pbody->h = 30;
	pbody->w = 20;
	health = 100;
	pbody->restitution = 0.1f;
	pbody->SetLimit(Vector2d(300.0f, 300.0f));
	isSelected = false;

	

	// SETING ANIMATIONS
	currentAnim = &jumpAnim;

	for (int i = 0; i < 36;i++)
		jumpAnim.PushBack({ 108,i * 60,54,64 });

	jumpAnim.PushBack({});
	jumpAnim.loop = true;
	jumpAnim.mustFlip = false;
	jumpAnim.speed = 0.1f;
	
}

Worm::~Worm()
{
}



void Worm::Update(float dt)
{
	position.x = pbody->x;
	position.y = pbody->y;

	if (app_->scene_intro->turnStarted) canMove = true;
	else canMove = false;
	if (isSelected && canMove)
	{
		if (app_->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			pbody->AddForce(Vector2d(-10.0f, 0.0f));
		}
		if (app_->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			pbody->AddForce(Vector2d(+10.0f, 0.0f));
		}
		if (app_->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			pbody->AddForce(Vector2d(0.0f, -50.0f));
		}
	}

	if (pbody->setPendingToDelete)
	{
		setPendingToDelete = true;
	}

	currentAnim->Update();
}

void Worm::Draw(SDL_Texture* tex)
{

	app_->renderer->Blit(tex, position.x - pbody->w, position.y - pbody->h, &currentAnim->GetCurrentFrame());
}

void Worm::Select()
{
	isSelected = true;
}

void Worm::UnSelect()
{
	isSelected = false;
}
