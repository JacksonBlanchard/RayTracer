#include "World.h"

World::World(IlluminationModel* _model)
{
	bgColor = color3(0.2, 0.7, 1.0);
	w_min = 0.0001;
	w_max = 1000;
	objects = {};
	lights = {};
	model = _model;
	defaultMaterial = new Air();
}

World::~World()
{
	objects.clear();
	lights.clear();
	delete defaultMaterial;
	delete root;
}

void World::addObject(shared_ptr<Object> object)
{
	objects.push_back(object);
}

void World::addLight(shared_ptr<LightSource> light)
{
	lights.push_back(light);
}

// check for ray intersection with object
Intersection World::spawn(Ray& ray)
{
	return root->getClosestIntersect(ray, w_min, w_max);

	Intersection hit;
	double closest_so_far = w_max;

	// check for an intersection with every object
	for (shared_ptr<Object> object : objects) {

		// check for an intersection with the object
		Intersection hit_test = object->intersect(ray, w_min, w_max);

		// if the ray intersected with an object
		if (hit_test.hit()) {
			// store the closest intersection
			if (hit_test.W() < closest_so_far) {
				closest_so_far = hit_test.W();
				hit = hit_test;
			}
		}
	}

	return hit;
}

// compute direct illumination using illumination model
color3 World::spawnShadow(Intersection& hit)
{
	// check for an intersection with every light
	for (shared_ptr<LightSource> light : lights) {

		// create shadow ray, starts at intersection and goes to light position
		Ray shadow(hit.Point(), (light->getPosition() - hit.Point()), hit.EndMat(), 0);

		Intersection shadowHit = spawn(shadow);

		// adjust shadow ray to ignore transparent objects
		/*
		// check for shadow ray intersection
		Intersection shadowHit;
		double closest_so_far = w_max;

		// check for an intersection with every object
		for (shared_ptr<Object> object : objects) {

			// check for an intersection with the object
			Intersection shadowHitTest = object->intersect(shadow, w_min, w_max);

			// if the ray intersected with an object and the object is not transparent
			if (shadowHitTest.hit() && shadowHitTest.EndMat()->Kt() == 0) {
				// store the closest intersection
				if (shadowHitTest.W() < closest_so_far) {
					closest_so_far = shadowHitTest.W();
					shadowHit = shadowHitTest;
				}
			}
		}
		*/

		// if shadow ray doesn't hit another object it is directly illuminated
		if (!shadowHit.hit()) {
			hit.AddLight(light);
		}
	}

	// get the local color value from the intersection
	return model->Illuminate(hit);
}

void World::buildTree()
{
	root = new KdTree(objects);
}

