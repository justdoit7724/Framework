#pragma once
#include "Scene.h"

namespace DX {
	class RenderScene : public Scene
	{
	public:

		void Update(float elapsed, float spf) override;

	};
}

