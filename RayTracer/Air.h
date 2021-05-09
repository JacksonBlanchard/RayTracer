#pragma once
#include "Material.h"

class Air : public Material
{
public:
	Air() : Material(0, 0, 1, color3(), color3()) {} // Default constructor

	double Kr() override { return 0; }
	double Kt() override { return 0; }
	double RefractiveIndex() override { return 1; }
};

