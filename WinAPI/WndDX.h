#pragma once
#include "IWindow.h"

class Window;
class Scene;

class WndDX : public IWindow
{
public:
	WndDX(HWND hwnd);
	~WndDX();

	void Update();

	void ReleaseKey(WPARAM wparam);
	void PressKey(WPARAM wparam);
	void Mouse_LBtnDown();
	void Mouse_LBtnUp();
	void Mouse_RBtnDown();
	void Mouse_RBtnUp();
	void Mouse_UpdatePt(LPARAM lparam);
	void Mouse_Wheel(WPARAM wparam);

	void WndProc(unsigned int msg, unsigned long long wparam, long long lparam) override;

private:
	WndDX() = delete;

	DX::Graphic* m_pEnv = nullptr;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_dContext;

	Scene* startScene = nullptr;
};

