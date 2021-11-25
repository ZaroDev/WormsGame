#include "PhysObject.h"
#include <math.h>
#include "p2Point.h"



PhysObject::PhysObject(Shape shape_, Type type_, int x_, int y_, float w_, float h_)
{
	shape = shape_;
	type = type_;
	x = x_;
	y = y_;
	w = w_;
	h = h_;

	if (shape == Shape::CIRCLE)
	{
		r = w / 2;
	}

}

PhysObject::~PhysObject()
{
}



