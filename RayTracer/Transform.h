#pragma once
#include "Ray.h"

class Transform
{
private:
	pos3 position;

public:
	Transform(); // Default Constructor
	Transform(pos3 _position); // Parameterized Constructor
	~Transform(); // Destructor
};

