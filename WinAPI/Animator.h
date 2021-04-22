#pragma once

#include "Component.h"

class Animator : public Component
{
public:
	Animator(Object* owner);
	~Animator();

	void Update(DX::Graphic* graphic) override;
	void Render(DX::Graphic* graphic)const override;
};


