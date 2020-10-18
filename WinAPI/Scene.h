#pragma once

class Keyboard;

class Scene
{
public:

	virtual void Update(float elapsed, float spf) = 0;

	virtual void Message(UINT msg) {};

	virtual void RButtonDown() {};
	virtual void RButtonUp() {};
	virtual void LButtonDown() {};
	virtual void LButtonUp() {};
	virtual void KeyDown(WPARAM wparam) {};
	virtual void KeyUp(WPARAM wparam) {};
	virtual void MouseMove(LPARAM lparam) {};

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

