#include "stdafx.h"
#include "ObjectMgr.h"
#include "Object.h"

void ObjectMgr::Register(Object* obj)
{
	assert(data.find(obj) == data.end());

	data.insert(obj);
}

void ObjectMgr::Remove(Object* obj)
{
	assert(data.find(obj) != data.end());

	data.erase(obj);
}
