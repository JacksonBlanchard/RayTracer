#include "Material.h"

Material::Material()
{
	kr = 0;
	kt = 0;
	ior = 1;
	diffuse_color = color3(1, 1, 1); // default white
	specular_color = color3(1, 1, 1); // default white
}

Material::Material(color3 _diffuse_color) : Material()
{
	diffuse_color = _diffuse_color;
}

Material::Material(color3 _diffuse_color, color3 _specular_color) : 
	Material(_diffuse_color)
{
	specular_color = _specular_color;
}

Material::Material(double _kr, color3 _diffuse_color) : Material(_diffuse_color)
{
	kr = _kr;
}

Material::Material(double _kr, double _kt, double _ior, color3 _diffuse_color, color3 _specular_color) : 
	Material(_diffuse_color, _specular_color)
{
	kr = _kr;
	kt = _kt;
	ior = _ior;
}

/*
vec3 Material::Scatter(vec3 view, Intersection i)
{

	return vec3();
}
*/

