#include "stdafx.h"
#include "ObjectPool.h"

PoolResource::PoolResource(void* data)
	:data(data)
{
}

void* PoolResource::Get() 
{
	assert(isUsing && "obj in pool is tempted to be used");

	return data;
}

void PoolResource::Retrieve()
{
	assert(isUsing && "obj in pool is tempted to be used");

	isUsing = false;
}

ObjectPool::ObjectPool(size_t objMemSize, int startCapacity)
	:objMemSize(objMemSize)
{
	for (int i = 0; i < startCapacity; ++i)
	{
		PoolResource* newObj = new PoolResource(std::malloc(objMemSize));
		pool.InsertBack(newObj);
	}


	curNode = pool.Head();
}

PoolResource* ObjectPool::Get()
{
	for (int i = 0; i < pool.Size(); ++i)
	{
		if (!curNode->data->isUsing)
		{
			auto temp = curNode;
			curNode = curNode->next ? curNode->next : pool.Head();
			temp->data->isUsing = true;
			return temp->data;
		}

		curNode = curNode->next ? curNode->next : pool.Head();
	}

	PoolResource* newObj = new PoolResource(std::malloc(objMemSize));
	pool.InsertBack(newObj);
	curNode = pool.Head();
	newObj->isUsing = true;
	return newObj;
}

int ObjectPool::Size()
{
	return pool.Size();
}

void ObjectPool::RetrieveAll()
{
	CustomSTL::SLNode<PoolResource*>* checkNode = pool.Head();
	while (checkNode)
	{
		checkNode->data->isUsing = false;
		checkNode = checkNode->next;
	}
}
