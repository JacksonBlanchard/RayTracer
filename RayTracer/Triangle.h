#pragma once
#include <cmath>
#include "Object.h"

class Triangle : public Object
{
protected:
	vec3 p1, p2, p3;
	double height, width;

public:
	Triangle(); // Default constructor
	Triangle(vec3 _p1, vec3 _p2, vec3 _p3);
	Triangle(vec3 _p1, vec3 _p2, vec3 _p3, Material* _material);

	void computeDimensions();
	virtual Intersection intersect(Ray ray, double w_min, double w_max) override;
	virtual void textureMap(pos3 point, double *u, double *v) override;
};

