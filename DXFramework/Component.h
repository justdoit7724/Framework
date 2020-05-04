#pragma once

#include "DX_info.h"

class Component
{
public:
	Component();

	virtual void Apply()const = 0;
};

