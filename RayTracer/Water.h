#pragma once
#include "Material.h"

class Water : public Material
{
private:
	//vec3 direction;
	//double wavelength;
	//double speed;
	//double steepness;
	//double amplitude;

public:
	Water() : Material(0.2, 0.8, 1.333, color3(), color3()) {} // Default constructor

	virtual color3 DiffuseColor(double u, double v) override;
};

