#pragma once
#include "Intersection.h"
#include "Material.h"

class IlluminationModel
{
public:
	virtual color3 Illuminate(Intersection i) = 0;
};

