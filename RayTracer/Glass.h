#pragma once
#include "Material.h"

class Glass : public Material
{
	//double kr = 1.0;   // coefficient for ideal (mirror) reflection
	//double kt = 1.0;   // coefficient for ideal refraction
	//double ior = 1.5;  // index of refraction
	//double Ks = 1.0;   // specular reflection coefficient
	//double shinyness = 20.0;   // Phong exponent

public:
	Glass() : Material(0, 1, 1.5, color3(), color3()) {} // Default constructor

	//double Kr() override { return 0; }
	//double Kt() override { return 1; }
	//double RefractiveIndex() override { return 1.5; }
};

