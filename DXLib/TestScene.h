#pragma once
#include "Scene.h"

namespace DX {
class Camera;
class UICanvas;
class UI;

	class TestScene :
		public Scene
	{
	public:
		TestScene();
		~TestScene();
		// Inherited via Scene
		virtual void Update(float elapsed, float spf) override;

	private:

		UICanvas* m_pCanvas;
		Camera* m_pCam;

		UI* m_pTestUI;
	};
}

