#include "PhongBlinn.h"

PhongBlinn::PhongBlinn()
{
	ka = 1.0;
	kd = 1.0;
	ks = 1.0;
	ke = 1.0;
}

color3 PhongBlinn::Illuminate(Intersection i)
{
	color3 ambient = ka * i.DiffuseColor();
	return ambient + DiffuseAndSpecular(i);
}

color3 PhongBlinn::DiffuseAndSpecular(Intersection i)
{
	color3 diffuse = color3(0, 0, 0);
	color3 specular = color3(0, 0, 0);

	// loop over all lights illuminating the intersection
	for (shared_ptr<LightSource> light : i.HitLights()) {
		// compute vector to light source and vector halfway between the light and view
		vec3 light_vec = unit_vector(light->getPosition() - i.Point());
		vec3 halfway_vec = unit_vector(light_vec + i.View());

		double LdotN = dot(light_vec, i.Normal()); // angle between light and normal vector
		double HdotN = dot(halfway_vec, i.Normal()); // angle between halfway and normal vector

		// intersection is facing the light
		if (LdotN >= 0.0) {
			diffuse += kd * i.DiffuseColor() * LdotN * light->getColor();

			// reflected vector is in view
			if (HdotN >= 0.0) {
				specular += ks * i.SpecularColor() * pow(HdotN, ke) * light->getColor();
			}
		}
	}

	return diffuse + specular;
}
