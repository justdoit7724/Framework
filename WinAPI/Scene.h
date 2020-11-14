#pragma once

class Keyboard;

class Scene
{
public:

	virtual void Update(float elapsed, float spf) = 0;

	virtual void Message(UINT msg) {}

	virtual void WM_RButtonDown() {}
	virtual void WM_RButtonUp() {}
	virtual void WM_LButtonDown() {}
	virtual void WM_LButtonUp() {}
	virtual void WM_KeyDown(WPARAM wparam) {}
	virtual void WM_KeyUp(WPARAM wparam) {}
	virtual void WM_MouseMove(LPARAM lparam) {}
	virtual void WM_Resize(int width, int height) {}

	const wchar_t* m_key;
protected:
	Scene(ID3D11Device* device, ID3D11DeviceContext* dContext, const wchar_t* key);
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_dContext;

	Keyboard* m_keyboard;
	XMFLOAT2 m_scnMousePos;
	bool m_bPressingRMouse;
	bool m_bPressingLMouse;
	int m_iWidth;
	int m_iHeight;
};


