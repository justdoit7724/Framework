#pragma once
#include "Scene.h"

namespace DX {
	class SetupScene : public Scene
	{
	public:
		SetupScene();

		void Update(float elapsed, float spf) override;
	};
}
