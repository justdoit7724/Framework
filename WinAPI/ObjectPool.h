#pragma once
#include <assert.h>

class ObjectPool;
class PoolResource
{
public:
	PoolResource(void* data);

	void* Get();
	
	void Retrieve();
	
private:
	friend ObjectPool;
	bool isUsing = false;
	void* data;
};

class ObjectPool
{
public:
	ObjectPool(size_t objMemSize, int startCapacity);
	

	PoolResource* Get();
	

	int Size();
	void RetrieveAll();

private:

	CustomSTL::SLNode<PoolResource*>* curNode = nullptr;
	CustomSTL::SLList<PoolResource*> pool;
	const size_t objMemSize;
};


