#pragma once
#include "CustomSTL.h"
#include <assert.h>

class ObjectPool;

class PoolResource
{
public:
	PoolResource(void* data) :data(data) {}

	void* Get()
	{
		assert(isUsing && "obj in pool is tempted to be used");

		return data;
	}
	void Retrieve()
	{
		assert(isUsing && "obj in pool is tempted to be used");

		isUsing = false;
	}

private:
	friend ObjectPool;
	bool isUsing = false;
	void* data;
};

class ObjectPool
{
public:
	ObjectPool(size_t objMemSize, int startCapacity)
		:objMemSize(objMemSize)
	{
		for (int i = 0; i < startCapacity; ++i)
		{
			PoolResource* newObj = new PoolResource(std::malloc(objMemSize));
			pool.InsertBack(newObj);
		}


		curNode = pool.Head();
	}

	PoolResource* Get()
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

	int Size() { return pool.Size(); }
	void RetrieveAll() {
		CustomSTL::SLNode<PoolResource*>* checkNode = pool.Head();
		while (checkNode)
		{
			checkNode->data->isUsing = false;
			checkNode = checkNode->next;
		}
	}

private:

	CustomSTL::SLNode<PoolResource*>* curNode = nullptr;
	CustomSTL::SLList<PoolResource*> pool;
	const size_t objMemSize;
};

