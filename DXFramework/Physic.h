#pragma once

#include "Geometrics.h"

class Object;

class Physic
{
public:
	static Object* Raycast(Geometrics::Ray ray, int mask);
};

