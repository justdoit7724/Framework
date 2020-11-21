#pragma once
#include "Scene.h"

class VisualAAScene :
	public Scene
{
public:
	VisualAAScene(ID3D11Device* device, ID3D11DeviceContext* dContext, const wchar_t* key);
	~VisualAAScene()override;

	void Update(float elapsed, float spf) override;

	void WM_RButtonDown()override;
	void WM_RButtonUp()override;
	void WM_KeyDown(WPARAM wparam)override;
	void WM_KeyUp(WPARAM wparam)override;
	void WM_MouseMove(LPARAM lparam)override;
	void WM_Resize(int width, int height) override;

	void SetResolution(int res);

private:

	DX::Camera* m_camera;
	DX::Buffer* m_cbPrimitive;
	DX::Object* m_dxQuad1;
	DX::Object* m_dxQuad2;
	DX::SamplingDraw* m_dxSampling;

	std::vector<DX::Object*> m_vObj;

};

