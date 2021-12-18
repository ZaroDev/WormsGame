#include "HandGun.h"

HandGun::HandGun() : Weapon()
{
	ammo = 10;
	name.Create("handgun");
}

HandGun::~HandGun()
{
}

void HandGun::Use()
{
	if(ammo > 0)
	{
		ammo--;
		//TODO shot
	}
}
