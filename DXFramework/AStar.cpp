#include "AStar.h"
#include "ObjectPool.h"

using namespace Pathfinding;
using namespace CustomSTL;

AStar::AStar(XMFLOAT3 firstPos, float interval, int widthCount, int depthCount)
	:widthCount(widthCount), depthCount(depthCount)
{
	assert(widthCount > 1 && depthCount > 1);

	int totalSize = widthCount * depthCount;

	wayPool = new ObjectPool(sizeof(DLNode<Path*>), totalSize/4);
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

	SetupPath();
}

bool AStar::FindPath(const Path* startPath, const Path* destPath, SLNode<const Path*>** outWay)const
{
	if (!startPath || !destPath || startPath->isBlock || destPath->isBlock)
		return false;

	ZeroMemory(checkMem, sizeof(bool) * widthCount * depthCount);

	std::queue<DLNode<const Path*>*> wayList;

	checkMem[startPath->idx] = true;
	auto destWayRes = wayPool->Get();
	DLNode<const Path*>* startNode = (DLNode<const Path*>*)destWayRes->Get();
	startNode->data = startPath;
	startNode->next = nullptr;
	startNode->prev = nullptr;

	wayList.push(startNode);


	while (wayList.size() > 0)
	{
		DLNode<const Path*>* curWay = wayList.front(); wayList.pop();

		for (auto nextPath : curWay->data->next)
		{
			if (checkMem[nextPath->idx] || nextPath->isBlock)
				continue;
			checkMem[nextPath->idx] = true;

			auto curWayRes = wayPool->Get();
			DLNode<const Path*>* nextWay = (DLNode<const Path*>*)(curWayRes->Get());
			nextWay->data = nextPath;
			nextWay->prev = curWay;
			nextWay->next = nullptr;
			curWay->next = nextWay;

			if (nextPath == destPath)
			{
				wayPool->RetrieveAll();

				SLList<const Path*> tempList;
				
				while (nextWay)
				{
					tempList.InsertFront(nextWay->data);
					nextWay = nextWay->prev;
				}

				tempList.Head()->DeepCopy(outWay);
				return true;
			}

			wayList.push(nextWay);
		}
	}

	wayPool->RetrieveAll();
	return false;
}

void Pathfinding::AStar::SetupPath()
{
	int totalSize = widthCount * depthCount;
	for (int i = 0; i < totalSize; ++i)
	{
		int x = i % widthCount;
		int y = i / widthCount;

		paths[i].next.clear();

		if (x > 0 && !paths[i - 1].isBlock) paths[i].next.push_back(&paths[i - 1]);
		if (x < widthCount - 1 && !paths[i + 1].isBlock) paths[i].next.push_back(&paths[i + 1]);
		if (y > 0 && !paths[i - widthCount].isBlock) paths[i].next.push_back(&paths[i - widthCount]);
		if (y < depthCount - 1 && !paths[i + widthCount].isBlock) paths[i].next.push_back(&paths[i + widthCount]);
	}
}
