#pragma once
#include "IlluminationModel.h"

class Phong : public IlluminationModel
{
private:
	double ka, kd, ks, ke;

public:
	Phong();
	Phong(double _ka, double _kd, double _ks, double _ke) : ka(_ka), kd(_kd), ks(_ks), ke(_ke) {};
	
	color3 Illuminate(Intersection i);
	color3 DiffuseAndSpecular(Intersection i);
};

