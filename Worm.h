
#include "Entity.h"
#include "Animation.h"
#include "HandGun.h"
#include "p2List.h"
#include "AirStrike.h"

class Worm : public Entity
{
public:
	Worm(p2Point<float> position_, Team team_, Application* app_);
	~Worm();

	void Update(float dt);
	void Draw(SDL_Texture* tex);


	void Select();
	void UnSelect();

private:
	bool isGrounded;
	bool canMove;

	Animation* currentAnim;

	Animation idleAnim;
	Animation jumpAnim;
	Animation atackAnim;
	Animation deadAnim;
	Animation talkAnim;

};

