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

	HandGun* gun = new HandGun();
	guns.add(gun);
	AirStrike* air = new AirStrike();
	guns.add(air);
	// SETING ANIMATIONS
	currentAnim = &deadAnim;
	currentWeapon = guns.getFirst();
	int offset = 10;

	for (int i = 0; i < 36; i++)
		jumpAnim.PushBack({ 108 + offset,i * 60 + offset,54 - offset,64 - offset });
	jumpAnim.loop = true;
	jumpAnim.mustFlip = false;
	jumpAnim.speed = 0.1f;


	for (int i = 0; i < 36; i++)
		idleAnim.PushBack({ 3 * 54 + offset,i * 60 + offset,54 - offset,64 - offset });
	idleAnim.loop = true;
	idleAnim.mustFlip = false;
	idleAnim.speed = 0.1f;

	for (int i = 0; i < 11; i++)
		deadAnim.PushBack({14 *54, i*60 - offset + 5,54 - offset,64 - offset });
	deadAnim.loop = true;
	deadAnim.mustFlip = true;
	deadAnim.speed = 0.07f;

	for (int i = 0; i < 32; i++)
		atackAnim.PushBack({ 9 * 54 + offset + 5,i * 60,54 - offset,64 - offset });
	atackAnim.loop = true;
	atackAnim.mustFlip = false;
	atackAnim.speed = 0.1f;
	atackAnim.pingpong = true;

	for (int i = 0; i < 5; i++)
		talkAnim.PushBack({ 5 * 54 + offset,(5 + i) * 60,54 - offset,64 - offset });
	talkAnim.loop = false;
	talkAnim.mustFlip = false;
	talkAnim.speed = 0.07f;
	talkAnim.pingpong = true;
	
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
		if (app_->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			currentWeapon->data->Use();
		}
	}
	if(app_->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		currentWeapon = currentWeapon->next;
		if (currentWeapon == nullptr)
			currentWeapon = guns.getFirst();
	}

	if (pbody->setPendingToDelete)
	{
		setPendingToDelete = true;
	}

	currentAnim->Update();
}

void Worm::Draw(SDL_Texture* tex)
{

	app_->renderer->Blit(tex, position.x - pbody->w / 2, position.y - pbody->h / 2, &currentAnim->GetCurrentFrame());
}

void Worm::Select()
{
	isSelected = true;
}

void Worm::UnSelect()
{
	isSelected = false;
}
