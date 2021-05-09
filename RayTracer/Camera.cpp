#include "Camera.h"

Camera::Camera(vec3 _position, vec3 _look_at, vec3 _local_up, double _image_width, double _image_height, double vfov, double _aspect_ratio)
{
	// compute view plane values
	double theta = vfov * 3.14159265 / 180.0; // vertical view angle
	double h = tan(theta / 2); // half height based on vertical view angle
	double viewPlane_height = 2.0 * h; // double the half height
	double viewPlane_width = viewPlane_height * _aspect_ratio; // multiply view plane height by aspect ratio
	double focal_length = 1.0; // 50 mm in Unity

	// compute local vectors
	vec3 n = unit_vector(_position - _look_at); // local backwards vector
	vec3 u = unit_vector(cross(_local_up, n)); // local right vector
	vec3 v = cross(n, u); // local up vector

	// compute and store class variables
	position = _position;
	look_at = _look_at;
	local_up = _local_up;
	horizontal = viewPlane_width * u; // horizontal view plane width vector
	vertical = viewPlane_height * v; // vertical view plane height vector
	upper_left_corner = position - (horizontal / 2) + (vertical / 2) - (n * focal_length); // world position of top left corner of view plane
	image_width = _image_width;
	// if height is not 0 (default), then compute using aspect ratio
	image_height = _image_height ? (_image_height) : (_image_width / _aspect_ratio);
}

