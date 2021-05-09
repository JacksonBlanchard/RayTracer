#pragma once
#include "vec3.h"

class Material
{
private:
	double kr, kt;	// reflection and transparency constants
	double ior;		// index of refraction of the material
	color3 diffuse_color, specular_color = color3(1,1,1); // white

public:
	Material();
	Material(color3 _diffuse_color);
	Material(color3 _diffuse_color, color3 _specular_color);
	Material(double _kr, color3 _diffuse_color);
	Material(double _kr, double _kt, double _ior, color3 _diffuse_color, color3 _specular_color);

	// getters
	virtual double Kr() { return kr; }
	virtual double Kt() { return kt; }
	virtual double RefractiveIndex() { return ior; }
	virtual color3 DiffuseColor(double u, double v) { return diffuse_color; }
	virtual color3 SpecularColor(double u, double v) { return specular_color; }
};

