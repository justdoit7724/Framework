#pragma once
#include "Component.h"

class VPShader : public Component
{
public:
	VPShader(ID3D11Device* device, const std::wstring VSfileName, const std::wstring PSfileName, const D3D11_INPUT_ELEMENT_DESC* layoutDesc, int layoutNum);

	void Apply(ID3D11DeviceContext* dContext);

private:
	ComPtr<ID3D11InputLayout> iLayout = nullptr;
	ComPtr<ID3D11VertexShader> vs = nullptr;
	ComPtr<ID3D11PixelShader> ps = nullptr;
};


class CShader : public Component
{
public:
	CShader(ID3D11Device* device, const std::wstring CSfileName);

	void Apply(ID3D11DeviceContext* dContext);

private:
	ComPtr<ID3D11ComputeShader> cs = nullptr;
};
