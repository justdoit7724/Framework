#pragma once

class Object;
class Ray;

class Physic
{
public:
	static Object* Raycast(Ray ray, int mask);
};

