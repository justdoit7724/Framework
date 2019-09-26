#pragma once
#include "DX_info.h"

class UI;
class Object;
class Component
{
public:
	Component();
	~Component();

	virtual void Apply()const = 0;
};

