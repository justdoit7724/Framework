#pragma once
#include "Component.h"

class VPShader : public Component
{
public:
	VPShader(std::string VSfileName, std::string PSfileName, const D3D11_INPUT_ELEMENT_DESC* layoutDesc, int layoutNum);

	void Apply();

private:
	ComPtr<ID3D11InputLayout> iLayout = nullptr;
	ComPtr<ID3D11VertexShader> vs = nullptr;
	ComPtr<ID3D11PixelShader> ps = nullptr;
};


class CShader : public Component
{
public:
	CShader(const std::string CSfileName);

	void Apply();

private:
	ComPtr<ID3D11ComputeShader> cs = nullptr;
};
