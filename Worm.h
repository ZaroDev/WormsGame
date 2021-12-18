
#include "Entity.h"
#include "Animation.h"

class Worm : public Entity
{
public:
	Worm(p2Point<float> position_, Team team_, Application* app_);
	~Worm();

	void Update(float dt);
	void Draw();


	void Select();
	void UnSelect();

private:
	bool isGrounded;

	SDL_Texture* sprites;
	Animation* currentAnim;

	Animation idleAnim;
	Animation walkAnim;
	Animation jumpAnim;
	Animation atackAnim;
};

