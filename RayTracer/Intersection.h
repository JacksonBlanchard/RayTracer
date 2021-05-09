#pragma once
#include "Ray.h"
#include "LightSource.h"
#include "Material.h"
//class Material; // necessary for circular dependencies

class Intersection
{
private:
	double w;		// distance along ray
	double u, v;	// texture coords
	Ray ray;		// incoming ray
	pos3 point;		// in world coords
	vec3 view;		// must be normalized
	vec3 normal;	// must be normalized
	bool outside;	// intersection on outside of object
	Material* endMaterial;		// material ray collides with
	vector<shared_ptr<LightSource>> hit_lights;

public:
	Intersection();
	Intersection(double _w, double _u, double _v, Ray _ray, vec3 _out_normal, Material* _material);
	~Intersection();

	Intersection& operator=(Intersection other);

	// Getters
	bool hit() const { return w > 0; } // default false, w = 0
	double W() const { return w; }
	double U() const { return u; }
	double V() const { return v; }
	pos3 Point() const { return point; }
	vec3 Normal() const { return normal; }
	vec3 View() const { return view; }
	bool Outside() const { return outside; }
	Material* RayMat() const { return ray.startMaterial; }
	Material* EndMat() const { return endMaterial; }
	vector<shared_ptr<LightSource>> HitLights() const { return hit_lights; }
	void AddLight(shared_ptr<LightSource> light) { hit_lights.push_back(light); }

	color3 DiffuseColor() { return endMaterial->DiffuseColor(u,v); }
	color3 SpecularColor() { return endMaterial->SpecularColor(u,v); }
};
