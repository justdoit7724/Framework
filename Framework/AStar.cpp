#include "stdafx.h"
#include "AStar.h"
#include "ObjectPool.h"

using namespace Pathfinding;

AStar::AStar(XMFLOAT3 firstPos, float interval, int widthCount, int depthCount)
	:widthCount(widthCount), depthCount(depthCount)
{
	assert(widthCount > 1 && depthCount > 1);

	int totalSize = widthCount * depthCount;

	paths = new Path[totalSize];
	checkMem = new bool[totalSize];

	for (int y = 0; y < depthCount; ++y)
	{
		for (int x = 0; x < widthCount; ++x)
		{
			int idx = x + y * widthCount;

			paths[idx].pos = /*XMFLOAT3(x, 0, y) * interval +*/ firstPos;
			paths[idx].pos.x += x * interval;
			paths[idx].pos.z += y * interval;

			paths[idx].idx = idx;
		}
	}

	for (int i=0; i<totalSize; ++i)
	{
		int x = totalSize % widthCount;
		int y = totalSize / widthCount;

		if (x > 0)
			paths[i].next.push_back(&paths[i-1]);
		if (x < widthCount - 1)
			paths[i].next.push_back(&paths[i+1]);
		if (y > 0)
			paths[i].next.push_back(&paths[i-widthCount]);
		if (y < depthCount - 1)
			paths[i].next.push_back(&paths[i+widthCount]);
	}
}

CustomSTL::DLNode<Path*>* AStar::FindPath(Path* startPath, Path* destPath)
{

	ZeroMemory(checkMem, sizeof(bool) * widthCount * depthCount);

	std::queue<CustomSTL::DLNode<Path*>*> wayList;

	wayList.push(new CustomSTL::DLNode<Path*>(destPath));


	while (wayList.size() > 0)
	{
		CustomSTL::DLNode<Path*>* curWay = wayList.front(); wayList.pop();
		Path* curListPath = curWay->data;

		if (checkMem[curListPath->idx])
			continue;

		checkMem[curListPath->idx] = true;
		for (auto nextPath : curListPath->next)
		{
		}
	}

	//debug
	return nullptr;
}
