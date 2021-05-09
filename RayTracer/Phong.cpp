#include "Phong.h"

Phong::Phong()
{
	ka = 1.0;
	kd = 1.0;
	ks = 1.0;
	ke = 1.0;
}

color3 Phong::Illuminate(Intersection i)
{
	color3 directIllumination = color3();
	if (i.Outside()) {	
		color3 ambient = ka * i.DiffuseColor();
		directIllumination += ambient + DiffuseAndSpecular(i);
	}
	return directIllumination;
}

color3 Phong::DiffuseAndSpecular(Intersection i)
{
	color3 diffuse = color3(0, 0, 0);
	color3 specular = color3(0, 0, 0);

	// loop over all lights illuminating the intersection
	for (shared_ptr<LightSource> light : i.HitLights()) {
		// compute vector to light source and vector of perfect reflection
		vec3 light_vec = unit_vector(light->getPosition() - i.Point());
		vec3 reflect_vec = unit_vector(reflectView(light_vec, i.Normal()));

		double LdotN = dot(light_vec, i.Normal()); // angle between light and normal vectors
		double RdotV = dot(reflect_vec, i.View()); // angle between reflection and view vectors

		// intersection is facing the light
		if (LdotN >= 0) {
			diffuse += kd * i.DiffuseColor() * LdotN * light->getColor();

			// reflected vector is in view
			if (RdotV >= 0) {
				specular += ks * i.SpecularColor() * pow(RdotV, ke) * light->getColor();
			}
		}
	}

	return diffuse + specular;
}

