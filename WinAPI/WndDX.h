#pragma once
#include "IWindow.h"

class Scene;
class Timer;

class WndDX : public IWindow
{
public:
	WndDX(HWND hwnd);
	~WndDX();

	void Update();

	void WndProc(unsigned int msg, unsigned long long wparam, long long lparam) override;

private:
	WndDX() = delete;

	DX::Graphic* m_pEnv = nullptr;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_dContext;

	Scene* startScene = nullptr;

	Timer* m_timer;
};

