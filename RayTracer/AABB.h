#pragma once
#include "Ray.h"

class AABB
{
public:
	vec3 minBounds, maxBounds;

	AABB(); // Default constructor

	void Encompass(vec3 obj_min, vec3 obj_max);
	double SplitValue(int axis);
	bool Intersect(Ray ray, double& tmin, double& tmax);
};

