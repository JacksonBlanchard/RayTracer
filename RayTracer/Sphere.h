#pragma once
#define _USE_MATH_DEFINES // M_PI
#include <cmath>
#include "Object.h"

class Sphere : public Object
{
private:
	vec3 center;
	double radius;

public:
	Sphere(); // Default constructor
	Sphere(vec3 _center, double _radius);
	Sphere(vec3 _center, double _radius, Material* _material);

	vec3 getCenter() const { return center; }
	double getRadius() const { return radius; }

	virtual Intersection intersect(Ray ray, double w_min, double w_max) override;
	virtual void textureMap(pos3 point, double *u, double *v) override;
};

