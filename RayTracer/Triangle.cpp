#include "Triangle.h"

Triangle::Triangle() : Object()
{
	p1 = vec3(-1, 0, -1);
	p2 = vec3(1, 0, 1);
	p3 = vec3(-1, 0, 1);
	computeDimensions();
}

Triangle::Triangle(vec3 _p1, vec3 _p2, vec3 _p3) : Triangle()
{
	p1 = _p1;
	p2 = _p2;
	p3 = _p3;
	computeDimensions();
}

Triangle::Triangle(vec3 _p1, vec3 _p2, vec3 _p3, Material* _material) : Triangle(_p1, _p2, _p3)
{
	material = _material;
}

void Triangle::computeDimensions()
{
	// compute height and width as max difference in z and x coords respectively
	// used for text coords
	height = max(abs(p1.z() - p2.z()), abs(p1.z() - p3.z()));
	width = max(abs(p1.x() - p2.x()), abs(p1.x() - p3.x()));

	// compute bounding box
	double minx = std::min(p1.x(), std::min(p2.x(), p3.x()));
	double miny = std::min(p1.y(), std::min(p2.y(), p3.y()));
	double minz = std::min(p1.z(), std::min(p2.z(), p3.z()));	
	aabb.minBounds = vec3(minx, miny, minz);
	double maxx = std::max(p1.x(), std::max(p2.x(), p3.x()));
	double maxy = std::max(p1.y(), std::max(p2.y(), p3.y()));
	double maxz = std::max(p1.z(), std::max(p2.z(), p3.z()));	
	aabb.maxBounds = vec3(maxx, maxy, maxz);
}

Intersection Triangle::intersect(Ray ray, double w_min, double w_max)
{
	vec3 e1 = p2 - p1;
	vec3 e2 = p3 - p1;
	vec3 t = ray.Origin() - p1;
	vec3 p = cross(ray.Direction(), e2);
	vec3 q = cross(t, e1);

	// ray is parallel to triangle
	if (dot(p, e1) == 0) {
		return Intersection();
	}

	double frac = 1 / dot(p, e1);
	double w = frac * dot(q, e2);
	double u = frac * dot(p, t);
	double v = frac * dot(q, ray.Direction());

	// intersection is behind ray or too far away
	if (w < w_min || w > w_max) {
		return Intersection();
	}
	else if (u < 0 || u > 1) {
		return Intersection();
	}
	else if (v < 0 || u + v > 1) { // intersection is outside of triangle
		return Intersection();
	}

	// intersection point and normal
	pos3 point = ((1-u-v) * p1) + (u * p2) + (v * p3);
	vec3 out_normal = unit_vector(cross(e2, e1));

	// texture coordinates
	double u_text, v_text;
	textureMap(point, &u_text, &v_text);

	// return intersection data
	return Intersection(w, u_text, v_text, ray, out_normal, material);
}

void Triangle::textureMap(pos3 point, double* u, double* v)
{
	*u = abs((point - p1).x() / width);
	*v = abs((point - p3).z() / height);
}

