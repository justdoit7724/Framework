#pragma once


#include <map>
#include <queue>
namespace DX {

class Object;
class SceneMgr;

	class Scene
	{
	public:

		virtual void Update(float elapsed, float spf) = 0;

		virtual void Message(UINT msg) {};

		const std::string key;
	protected:

	private:
	};
}

