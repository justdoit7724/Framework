#pragma once

class Keyboard;

class Scene
{
public:

	virtual void Update(float elapsed, float spf) = 0;

	virtual void Message(UINT msg) {};

	virtual void ReleaseKey(WPARAM wparam);
	virtual void PressKey(WPARAM wparam);
	virtual void Mouse_LBtnDown();
	virtual void Mouse_LBtnUp();
	virtual void Mouse_RBtnDown();
	virtual void Mouse_RBtnUp();
	virtual void Mouse_UpdatePt(LPARAM lparam);
	virtual void Mouse_Wheel(WPARAM wparam);

	const wchar_t* m_key;
protected:
	Scene(ID3D11Device* device, ID3D11DeviceContext* dContext, const wchar_t* key);
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_dContext;

	Keyboard* m_keyboard;
	XMFLOAT2 m_scnMousePos;
	bool m_bPressingRMouse;
	bool m_bPressingLMouse;
};


