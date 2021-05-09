#include "Sphere.h"

Sphere::Sphere() : Object()
{
	center = pos3(); // origin
	radius = 1.0;
	aabb.minBounds = center - vec3(radius, radius, radius);
	aabb.maxBounds = center + vec3(radius, radius, radius);
}

Sphere::Sphere(vec3 _center, double _radius) : Sphere()
{
	center = _center;
	radius = _radius;
	aabb.minBounds = _center - vec3(_radius, _radius, _radius);
	aabb.maxBounds = _center + vec3(_radius, _radius, _radius);
}

Sphere::Sphere(vec3 _center, double _radius, Material* _material) : Sphere(_center, _radius)
{
	material = _material;
}

Intersection Sphere::intersect(Ray ray, double w_min, double w_max)
{
	vec3 oc = ray.Origin() - getCenter();
	//double a = ray.getDirection().length_squared();	
	double b = 2.0 * dot(oc, ray.Direction());
	double c = dot(oc, oc) - radius * radius;

	double discriminant = (b * b) - (4.0 * c);
	if (discriminant < 0) {
		return Intersection();
	}

	double root = sqrt(discriminant);
	double minus_root = (-b - root) / 2.0;
	double plus_root = (-b + root) / 2.0;

	double w;
	// store the smallest valid root
	if (minus_root > w_min && minus_root < w_max) {
		w = minus_root;
	}
	else if (plus_root > w_min && plus_root < w_max) {
		w = plus_root;
	}
	else {
		return Intersection();
	}

	// intersection point and normal
	pos3 point = ray.at(w);
	vec3 out_normal = unit_vector(point - getCenter());	

	// texture coordinates
	double u, v;
	textureMap(point, &u, &v);

	//if(ray.startMaterial == material)

	// return the intersection
	return Intersection(w, u, v, ray, out_normal, material);
}

void Sphere::textureMap(pos3 point, double* u, double* v)
{
	// compute the azimuthal angle
	// -π < theta <= π
	// angle increases clockwise as viewed from above,
	// which is opposite of what we want, but we'll fix it later.
	double theta = atan2(point.x(), point.z());
	// compute the polar angle
	// 0 <= phi <= π
	double phi = acos(point.y() / radius);	
	// -0.5 < raw_u <= 0.5
	double raw_u = theta / (2 * M_PI);
	// 0 <= u < 1
	// here's also where we fix the direction of u. Subtract it from 1,
	// so that it increases counterclockwise as viewed from above.
	*u = 1 - (raw_u + 0.5);
	// we want v to be 0 at the south pole of the sphere,
	// and 1 at the north pole, so we have to "flip it over"
	// by subtracting it from 1.
	*v = 1 - phi / M_PI;
}

