#pragma once
#include "Material.h"

class Checkerboard : public Material
{
private:
	color3 color1;
	color3 color2;
	double scale;

public:
	Checkerboard(); // Default constructor
	Checkerboard(double _scale);
	Checkerboard(double _scale, color3 _color1, color3 _color2); // Parameterized constructor

	virtual color3 DiffuseColor(double u, double v) override;
};

