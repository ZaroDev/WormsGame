#include "AirStrike.h"

AirStrike::AirStrike()
{
	name.Create("airstrike");
	ammo = 1;
}

AirStrike::~AirStrike()
{
}

void AirStrike::Use()
{
	if (ammo > 0)
	{
		ammo--;
		//TODO shot
	}
}
