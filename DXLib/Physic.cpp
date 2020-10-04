
#include "pch.h"

#include "Physic.h"
#include "Object.h"
#include "Collider.h"
#include "LayerMask.h"

using namespace DX;

Object* Physic::Raycast(const std::vector<Object*>& objs, Geometrics::Ray ray, int mask)
{
	for (Object* obj : objs)
	{
		if (!LayerCheck(obj->Layer(), mask))
			continue;

		XMFLOAT3 hitPt;
		if (obj->collider->IsHit(ray, &hitPt))
		{
			return obj;
		}
	}

	return nullptr;
}
