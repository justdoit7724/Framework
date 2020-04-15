#pragma once


namespace Pathfinding
{
	class Path
	{
	public:
		XMFLOAT3 pos;
		bool isBlock=false;
		int idx;
		std::vector<Path*> next;
	};


	class AStar
	{
	public:
		AStar(XMFLOAT3 firstPos, float interval, int widthCount, int depthCount);

		CustomSTL::DLNode<Path*>* FindPath(Path* startPath, Path* destPath);

		Path* GetPaths() { return paths; }

		const int widthCount;
		const int depthCount;

	private:
		Path* paths;
		bool* checkMem;
	};
}