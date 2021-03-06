#pragma once
#include "Math.h"

class ObjectPool;

namespace Pathfinding
{
	class Path
	{
	public:
		DirectX::XMFLOAT3 pos;
		bool isBlock = false;
		int idx;
		std::vector<Path*> next;
	};


	class AStar
	{
	public:
		AStar(DirectX::XMFLOAT3 firstPos, float interval, int widthCount, int depthCount);

		bool FindPath(const Path* startPath, const Path* destPath, CustomSTL::SLNode<const Path*>** outWay)const;
		void SetupPath();
		const Path* GetPaths() { return paths; }

		const int widthCount;
		const int depthCount;

	private:
		Path* paths;
		bool* checkMem;
		ObjectPool* wayPool;
	};
}
