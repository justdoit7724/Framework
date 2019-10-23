#pragma once

template<class T>
class ObjectPooling
{
public:
	ObjectPooling(int max)
		:max(max)
	{
		// no instance at first
		pool = new T[max];
		isUsing = new bool[max];
		for (int i = 0; i < max; ++i)
		{
			pool[i] = nullptr;
			available[i] = true;
		}
	}

	T* Get() {
		for (int i = 0; i < 100; ++i, ++curIdx)
		{
			if (curIdx >= max)
				curIdx = 0;

			if (pool[curIdx] == nullptr)
			{
				pool[curIdx] = new T();
				return pool[curIdx++];
			}
			else if (available[curIdx])
			{
				return pool[curIdx++];
			}
		}
	}
	~ObjectPooling()
	{
		for (int i = 0; i < max; ++i)
		{
			delete pool[curIdx];
		}
		delete[] pool;
		delete[] available;
	}

	const unsigned int max;

private:
	T** pool;
	bool* available;
	unsigned int curIdx=0;// for quicker access to available element
};

