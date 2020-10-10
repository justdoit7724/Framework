#pragma once
#include "Scene.h"

class Object;

class StartScene :
	public Scene
{
public:
	StartScene(ID3D11Device* device, ID3D11DeviceContext* dContext, const wchar_t* key);
	~StartScene();

	void Update(float elapsed, float spf) override;

private:

	DX::DirectionalLight* m_dLight;
	DX::Camera* m_camera;
	DX::Buffer* m_cbEye;

	DX::Object* m_dxBlueBox;
	DX::Object* m_dxGreenBox;
	DX::Object* m_dxRedBox;

	std::vector<DX::Object*> m_vObj;

	// Inherited via Scene
	void WndProc(UINT MSG, WPARAM wparam, LPARAM lparam)override;
};

