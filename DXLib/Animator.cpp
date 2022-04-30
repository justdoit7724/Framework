#include "stdafx.h"
#include "Animator.h"



Animator::Animator(Object* owner)
	:Component(CMPNT_KIND::Animator, owner)
{
}

Animator::~Animator()
{
}

void Animator::Update(DX::Graphic* graphic)
{
}

void Animator::Render(DX::Graphic* graphic) const
{
}
