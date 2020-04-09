#pragma once


namespace Pathfinding
{
	class Path
	{
	public:
		XMFLOAT3 pos;
		bool isBlock=false;
		std::vector<Path*> next;
	};


	class AStar
	{
	public:
		AStar(XMFLOAT3 firstPos, float interval, int widthCount, int depthCount);

		void FindPath(OUT std::list<Path*>& paths);

		Path* const* GetPaths() { return paths; }

	private:
		Path** paths;

	};
}