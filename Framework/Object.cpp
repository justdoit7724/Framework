#include "Object.h"
#include "Shader.h"
#include "CustomFormat.h"
#include "Shape.h"
#include "Network.h"
#include "TextureMgr.h"
#include "Transform.h"

int CalculateMaxMiplevel(int width, int height)
{
	int maxResol = max(width, height);
	int maxMiplevel = 1;

	while (maxResol > 1)
	{
		maxResol = floor(maxResol * 0.5f);
		maxMiplevel++;
	}

	return maxMiplevel;
}

Object::Object(IGraphic* graphic, Shape* shape, XMFLOAT3 mDiffuse, XMFLOAT3 mAmbient, XMFLOAT3 mSpec, float sP, XMFLOAT3 r, std::string imageName, bool mipmap, int zOrder)
	: isMipmap(mipmap), zOrder(zOrder), shape(shape)
{
	ID3D11Device* device = graphic->Device();

	transform = new Transform();

	shader = new VPShader(device, L"StandardVS.cso", L"StandardPS.cso", std_ILayouts, ARRAYSIZE(std_ILayouts));

	cb_vs_property = new ConstantBuffer<VS_Property>(device);
	cb_ps_dLights = new ConstantBuffer<SHADER_DIRECTIONAL_LIGHT>(device);
	cb_ps_pLights = new ConstantBuffer<SHADER_POINT_LIGHT>(device);
	cb_ps_sLights = new ConstantBuffer<SHADER_SPOT_LIGHT>(device);
	cb_ps_eyePos = new ConstantBuffer<XMFLOAT3>(device);
	cb_ps_material = new ConstantBuffer<ShaderMaterial>(device);

	material = new ShaderMaterial(mDiffuse, mAmbient, mSpec, sP, r);

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	r_assert(
		device->CreateSamplerState(&samplerDesc, &bodySameplerState)
	);

#pragma region MIPMAPPING

	if (mipmap)
	{
		TextureMgr::Instance()->Load(graphic, imageName);
		ID3D11Texture2D* image = TextureMgr::Instance()->GetTexture(imageName);
		ComPtr<ID3D11Texture2D> mipmapTexture = nullptr;
		D3D11_TEXTURE2D_DESC imageDesc;
		image->GetDesc(&imageDesc);

		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.ArraySize = 1;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		texDesc.CPUAccessFlags = 0;
		texDesc.Format = imageDesc.Format;
		texDesc.Width = imageDesc.Width;
		texDesc.Height = imageDesc.Height;
		texDesc.MipLevels = CalculateMaxMiplevel(imageDesc.Width, imageDesc.Height);
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.SampleDesc = { 1,0 };
		r_assert(device->CreateTexture2D(&texDesc, nullptr, mipmapTexture.GetAddressOf()));

		ComPtr<ID3D11Texture2D> stagTex;
		D3D11_TEXTURE2D_DESC stagDesc;
		stagDesc.ArraySize = 1;
		stagDesc.BindFlags = 0;
		stagDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		stagDesc.Format = imageDesc.Format;
		stagDesc.Width = imageDesc.Width;
		stagDesc.Height = imageDesc.Height;
		stagDesc.MipLevels = 1;
		stagDesc.MiscFlags = 0;
		stagDesc.Usage = D3D11_USAGE_STAGING;
		stagDesc.SampleDesc = { 1,0 };
		r_assert(device->CreateTexture2D(&stagDesc, nullptr, &stagTex));

		graphic->DContext()->CopyResource(stagTex.Get(), image);

		D3D11_MAPPED_SUBRESOURCE mapped;
		r_assert(graphic->DContext()->Map(stagTex.Get(), 0, D3D11_MAP_READ, 0, &mapped));
		UINT* arr = new UINT[(mapped.RowPitch / (float)sizeof(UINT)) * imageDesc.Height];
		ZeroMemory(arr, mapped.RowPitch*imageDesc.Height);
		CopyMemory(arr, mapped.pData, mapped.RowPitch*imageDesc.Height);
		graphic->DContext()->Unmap(stagTex.Get(), 0);

		graphic->DContext()->UpdateSubresource(mipmapTexture.Get(), 0, &CD3D11_BOX(0, 0, 0, imageDesc.Width, imageDesc.Height, 1), arr, mapped.RowPitch, mapped.DepthPitch);
		delete[] arr;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = imageDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		r_assert(device->CreateShaderResourceView(mipmapTexture.Get(), &srvDesc, bodySRV.GetAddressOf()));
		graphic->DContext()->GenerateMips(bodySRV.Get());
	}
	else {
		bodySRV = TextureMgr::Instance()->Get(imageName);
	}
#pragma endregion

}

Object::~Object()
{
	delete transform;
	delete shape;
	delete shader;
	delete cb_vs_property;
	delete cb_ps_dLights;
	delete cb_ps_pLights;
	delete cb_ps_sLights;
	delete cb_ps_eyePos;
	delete cb_ps_material;
	delete material;
}

void Object::Update()
{
}

void Object::Render(IGraphic* graphic, Camera* camera, const SHADER_DIRECTIONAL_LIGHT* dLight, const SHADER_POINT_LIGHT* pLight, const SHADER_SPOT_LIGHT* sLight, const XMMATRIX& texMat)
{
	ID3D11DeviceContext* dContext = graphic->DContext();
	XMMATRIX vpMat = camera->ViewMat()*camera->ProjMat(zOrder);

	shader->SetPipline(dContext);

	// TRANSFORM
	cb_vs_property->VSSetData(dContext, &VS_Property(transform, vpMat));

	// LIGHTS
	cb_ps_dLights->PSSetData(dContext, dLight, 0);
	cb_ps_pLights->PSSetData(dContext, pLight, 1);
	cb_ps_sLights->PSSetData(dContext, sLight, 2);

	// EYE
	cb_ps_eyePos->PSSetData(dContext, &camera->Pos(), 3);

	// MATERIAL
	cb_ps_material->PSSetData(dContext, material, 4);

	// TEXTURE
	dContext->PSSetShaderResources(0, 1, bodySRV.GetAddressOf());
	dContext->PSSetSamplers(0, 1, bodySameplerState.GetAddressOf());

	graphic->SetRasterizerState();
	graphic->SetDepthStencilState();
	shape->Render(dContext);
}
