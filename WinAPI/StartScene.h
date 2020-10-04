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

	std::vector<DX::Object*> m_vObj;
};

