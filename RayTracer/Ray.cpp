#include "Ray.h"

Ray::Ray()
{
	depth = 0;
	origin = vec3(0, 0, 0);
	direction = vec3(0, 0, 1);
	startMaterial = nullptr;
}

Ray::Ray(vec3 _origin, vec3 _direction) : Ray()
{
	origin = _origin;
	direction = unit_vector(_direction);
}

Ray::Ray(vec3 _origin, vec3 _direction, Material* _startMaterial, int _depth) : Ray(_origin, _direction)
{
	startMaterial = _startMaterial;
	depth = _depth;
}

Ray::~Ray()
{
	startMaterial = nullptr;
}

