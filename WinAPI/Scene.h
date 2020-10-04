#pragma once

class Scene
{
public:

	virtual void Update(float elapsed, float spf) = 0;

	virtual void Message(UINT msg) {};

	const wchar_t* m_key;
protected:
	Scene(ID3D11Device* device, ID3D11DeviceContext* dContext, const wchar_t* key);
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_dContext;

};


