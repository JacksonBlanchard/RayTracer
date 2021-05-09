#pragma once
#include "vec3.h"
#include "Material.h"

class Ray
{
private:
	int depth;
	vec3 origin;
	vec3 direction;

public:
	Material* startMaterial;

	Ray(); // Default constructor
	Ray(vec3 _origin, vec3 _direction); // Parameterized constructor (viewplane rays)
	Ray(vec3 _origin, vec3 _direction, Material* _startMaterial, int _depth); // (recursive rays)
	~Ray(); // Destructor

	int Depth() const { return depth; }
	vec3 Origin() const { return origin; }
	vec3 Direction() const { return direction; }
	vec3 at(double w) const { return origin + (w * direction); }
};

