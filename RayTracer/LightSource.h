#pragma once
#include "vec3.h"

class LightSource
{
private:
	vec3 position;
	color3 color;
	double intensity;

public:
	LightSource(vec3 _position, color3 _color) : position(_position), color(_color), intensity(1) {}

	LightSource(vec3 _position, color3 _color, double _power) : LightSource(_position, _color) {
		intensity = _power;
	}

	pos3 getPosition() { return position; }
	color3 getColor() { return color * intensity; }
};

