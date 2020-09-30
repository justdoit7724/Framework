#pragma once

#include "Singleton.h"

namespace DX
{
class Camera;

	class CameraMgr :
		public Singleton<CameraMgr>
	{
	public:
		void Add(const Camera* camera);
		void Remove(const Camera* camera);
		void SetMain(const Camera* camera) { mainCamera = camera; }
		const Camera* Main() {
			return mainCamera;
		};
		std::unordered_set<const Camera*>& Data() { return data; }

	private:
		const Camera* mainCamera = nullptr;
		std::unordered_set<const Camera*> data;
	};
}

