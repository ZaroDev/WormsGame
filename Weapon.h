#pragma once
#include "SString.h"

class Weapon
{
public:
	Weapon() {}
	~Weapon(){}

	virtual void Use(){}
	int ammo;
	SString name;
};

