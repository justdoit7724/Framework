#pragma once
#include "Object.h"

class Transform;

class Token : public Object
{
public:
	Token();

private:
	Transform* transform;
};