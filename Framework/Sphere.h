#pragma once
#include "Shape.h"
class Sphere :
	public Shape
{
public:
	Sphere(ID3D11Device* device, const int numSubDivision);
};

