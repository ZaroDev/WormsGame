
#include "Entity.h"
#include "Animation.h"

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


	Animation* currentAnim;

	Animation idleAnim;
	Animation walkAnim;
	Animation jumpAnim;
	Animation atackAnim;
};

