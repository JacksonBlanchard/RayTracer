#include "AABB.h"

AABB::AABB()
{
	minBounds = vec3(999, 999, 999);
	maxBounds = vec3(-999, -999, -999);
}

void AABB::Encompass(vec3 obj_min, vec3 obj_max)
{
	minBounds = vec3(std::min(minBounds.x(), obj_min.x()),
		std::min(minBounds.y(), obj_min.y()),
		std::min(minBounds.z(), obj_min.z()));
	maxBounds = vec3(std::max(maxBounds.x(), obj_max.x()),
		std::max(maxBounds.y(), obj_max.y()),
		std::max(maxBounds.z(), obj_max.z()));
}

double AABB::SplitValue(int axis)
{
	return (minBounds[axis] + maxBounds[axis]) / 2;
}

bool AABB::Intersect(Ray ray, double& tmin, double& tmax)
{
	// how does this work lol
	double t1 = (minBounds.x() - ray.Origin().x()) * (1 / ray.Direction().x());
	double t2 = (maxBounds.x() - ray.Origin().x()) * (1 / ray.Direction().x());
	double t3 = (minBounds.y() - ray.Origin().y()) * (1 / ray.Direction().y());
	double t4 = (maxBounds.y() - ray.Origin().y()) * (1 / ray.Direction().y());
	double t5 = (minBounds.z() - ray.Origin().z()) * (1 / ray.Direction().z());
	double t6 = (maxBounds.z() - ray.Origin().z()) * (1 / ray.Direction().z());

	tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	// If tmax < 0, ray is intersecting AABB, but whole AABB is behind us.
	if (tmax < 0) {
		return false;
	}

	// If tmin > tmax, ray doesn't intersect AABB.
	if (tmin > tmax) {
		return false;
	}

	return true;
}

