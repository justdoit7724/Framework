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

	void EnableDLight(bool b);
	void EnablePLight(bool b);
	void EnableSLight(bool b);

	void RButtonDown()override;
	void RButtonUp()override;
	void KeyDown(WPARAM wparam)override;
	void KeyUp(WPARAM wparam)override;
	void MouseMove(LPARAM lparam)override;

private:

	DX::DirectionalLight* m_dLight;
	DX::PointLight* m_pLight;
	DX::SpotLight* m_sLight;
	DX::Camera* m_camera;
	DX::Buffer* m_cbEye;

	DX::Object* m_dxBlueBox;
	DX::Object* m_dxGreenBox;
	DX::Object* m_dxRedBox;
	DX::Object* m_dxPLightBlub;
	DX::Object* m_dxSLightBlub;

	std::vector<DX::Object*> m_vObj;

};

