#include "Object.h"

Object::Object()
{
	material = nullptr;
	aabb = AABB();
}

Object::~Object()
{
	material = nullptr;
}
