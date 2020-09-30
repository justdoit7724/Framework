#pragma once

#include "Geometrics.h"

namespace DX {
class Object;

	class Physic
	{
	public:
		static Object* Raycast(Geometrics::Ray ray, int mask);
	};
}

