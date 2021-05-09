#pragma once
#include "Ray.h"

class Camera
{
private:
	pos3 position;
	vec3 look_at, local_up;
	pos3 upper_left_corner;
	vec3 horizontal, vertical;
	double image_width, image_height;

public:
	Camera(vec3 _position, vec3 _look_at, vec3 _local_up, double _image_width, double _image_height, double vfov, double _aspect_ratio);

	double GetImageWidth() const { return image_width; }
	double GetImageHeight() const { return image_height; }

	Ray getRay(double u, double v) const {
		return Ray(position, upper_left_corner + (u * horizontal) - (v * vertical) - position);
	}
};

