#pragma once
#include "Scene.h"

namespace DX {
	class DebuggingScene :
		public Scene
	{
	public:
		DebuggingScene();
		~DebuggingScene();

		void Update(float elapsed, float spf)override;
	};
}