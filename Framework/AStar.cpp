#include "stdafx.h"
#include "AStar.h"

using namespace Pathfinding;

AStar::AStar(XMFLOAT3 firstPos, float interval, int widthCount, int depthCount)
{
	assert(widthCount > 1 && depthCount > 1);

	paths = new Path*[depthCount];
	for (int i = 0; i < depthCount; ++i)
	{
		paths[i] = new Path[widthCount];
	}

	for (int y = 0; y < depthCount; ++y)
	{
		for (int x = 0; x < widthCount; ++x)
		{
			paths[y][x].pos = XMFLOAT3(x, 0, y) * interval + firstPos;
		}
	}

	for (int y = 0; y < depthCount; ++y)
	{
		for (int x = 0; x < widthCount; ++x)
		{
			if (x > 0)
				paths[y][x].next.push_back(&paths[y][x-1]);
			if (x < widthCount - 1)
				paths[y][x].next.push_back(&paths[y][x + 1]);
			if (y > 0)
				paths[y][x].next.push_back(&paths[y - 1][x]);
			if (y < depthCount - 1)
				paths[y][x].next.push_back(&paths[y + 1][x]);
		}
	}
}

void Pathfinding::AStar::FindPath(OUT std::list<Path*>& pathList)
{

}
