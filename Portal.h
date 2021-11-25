#pragma once
#include "PhysObject.h"


class Portal
{
public:
	Portal();
	~Portal();
	void SetPortal1(float x, float y, float w, float h, Shape shape, Type type, PortalType ptype, const char* name);
	void SetPortal2(float x, float y, float w, float h, Shape shape, Type type, PortalType ptype, const char* name);

	PhysObject* p1;
	PhysObject* p2;
	void Teletransport(PhysObject* entry, PhysObject* obj);
};
