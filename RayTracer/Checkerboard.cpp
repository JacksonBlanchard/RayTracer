#include "Checkerboard.h"
#include "Intersection.h"

Checkerboard::Checkerboard() : Material()
{
	color1 = color3(1, 0, 0); // default red
	color2 = color3(1, 1, 0); // default yellow
	scale = 2;
}

Checkerboard::Checkerboard(double _scale) : Checkerboard()
{
	scale = _scale;
}

Checkerboard::Checkerboard(double _scale, color3 _color1, color3 _color2) : Checkerboard(_scale)
{
	color1 = _color1;
	color2 = _color2;
}

color3 Checkerboard::DiffuseColor(double u, double v)
{
	int row = (int)(u * scale);
	int col = (int)(v * scale);

	if ((row + col) % 2 == 0) {
		return color1;
	}
	else {
		return color2;
	}
}
