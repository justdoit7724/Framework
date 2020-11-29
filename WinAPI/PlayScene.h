#pragma once
#include "Scene.h"

class Object;

class PlayScene :
	public Scene
{
public:
	PlayScene(ID3D11Device* device, ID3D11DeviceContext* dContext, const wchar_t* key);
	~PlayScene() override;

	void Update(float elapsed, float spf) override;

	void WM_LButtonDown()override;
	void WM_RButtonDown()override;
	void WM_RButtonUp()override;
	void WM_KeyDown(WPARAM wparam)override;
	void WM_KeyUp(WPARAM wparam)override;
	void WM_MouseMove(LPARAM lparam)override;

private:

	DX::DirectionalLight* m_dLight;
	DX::Camera* m_camera;
	DX::Buffer* m_cbEye;

	DX::Object* m_dxRedBox;

	ID3D11ShaderResourceView* m_dxRedSRV;
	ID3D11ShaderResourceView* m_dxBlueSRV;
	ID3D11ShaderResourceView* m_dxGreenSRV;
	ID3D11ShaderResourceView* m_dxWhiteSRV;
	ID3D11ShaderResourceView* m_dxNormSRV;

	ID3D11SamplerState* m_dxSamp;

	std::vector<DX::Object*> m_vObj;

};


