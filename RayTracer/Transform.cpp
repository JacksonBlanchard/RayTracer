#include "Transform.h"

Transform::Transform()
{
	position = vec3(0, 0, 0);
}

Transform::Transform(pos3 _position)
{
	position = _position;
}

Transform::~Transform()
{
	return;
}
