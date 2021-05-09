#include "Intersection.h"

Intersection::Intersection() {
	w = 0;
	u = 0;
	v = 0;
	ray = Ray();
	point = pos3();
	view = vec3();
	normal = vec3();
	outside = true;
	endMaterial = nullptr;
	hit_lights = {};
}

Intersection::Intersection(double _w, double _u, double _v, 
	Ray _ray, vec3 _out_normal, Material* _endMaterial) : 
	w(_w), u(_u), v(_v), ray(_ray), endMaterial(_endMaterial)
{
	point = _ray.at(_w);
	view = unit_vector(_ray.Origin() - point);
	outside = dot(view, _out_normal) > 0;
	normal = outside ? _out_normal : -_out_normal;
	hit_lights = {};
}

Intersection::~Intersection()
{
	endMaterial = nullptr;
	hit_lights.clear();
}

Intersection& Intersection::operator=(Intersection other)
{
	w = other.w;
	u = other.u;
	v = other.v;
	ray = other.ray;
	point = other.point;
	view = other.view;
	normal = other.normal;
	endMaterial = other.endMaterial;
	hit_lights = other.hit_lights;
	return *this;
}

