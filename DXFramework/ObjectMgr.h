#pragma once

#include "Singleton.h"
#include "LayerMask.h"

class Object;

class ObjectMgr : public Singleton<ObjectMgr>
{
public:
	void Register(Object* obj);
	void Remove(Object* obj);
	std::unordered_set<Object*> Data() { return data; }

private:
	std::unordered_set<Object*> data;
};