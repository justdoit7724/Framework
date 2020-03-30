#include "stdafx.h"
#include "Physic.h"
#include "ObjectMgr.h"
#include "Object.h"
#include "Collider.h"

Object* Physic::Raycast(Ray ray, int mask)
{
	for (Object* obj : ObjectMgr::Instance()->Data())
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
