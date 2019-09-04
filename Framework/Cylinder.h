#pragma once
#include "Shape.h"
class Cylinder :
	public Shape
{
public:
	Cylinder(ID3D11Device* device, const float height, const float rad, const int intensity);
};

