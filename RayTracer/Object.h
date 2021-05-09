#pragma once
#include "Material.h"
#include "Intersection.h"
#include "AABB.h"

class Object
{
protected:	
	Material* material;
	AABB aabb;

public:
	Object();
	~Object();

	Material* getMaterial() { return material; }
	AABB getAABB() { return aabb; }
	vec3 getMin() { return aabb.minBounds; }
	vec3 getMax() { return aabb.maxBounds; }

	virtual Intersection intersect(Ray ray, double w_min, double w_max) = 0;
	virtual void textureMap(pos3 point, double *u, double *v) = 0;
};
