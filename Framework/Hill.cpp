#include "Hill.h"
#include "Shader.h"
#include "Resource.h"
#include "TextureMgr.h"
#include "CustomFormat.h"
#include "Network.h"
#include "Debugging.h"

Hill::Hill(IGraphic* graphic, int n, int m, XMFLOAT2 heightRange, ID3D11ShaderResourceView *const* heightMap)
	:Shape()
{
	ID3D11Device* device = graphic->Device();
	ID3D11DeviceContext* dContext = graphic->DContext();

#pragma region define x, z

	float dx = 1.0f / (n - 1);
	float dz = 1.0f / (m - 1);
	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	std::vector<Vertex> vertice(m * n);

	for (UINT i = 0; i < m; ++i)
	{
		float z = i * dz - 0.5f;
		for (UINT j = 0; j < n; ++j)
		{
			int idx = i*n + j;
			float x = -0.5f + j * dx;
			vertice[idx].pos = XMFLOAT3(x, 0.0f, z);

			vertice[idx].n = XMFLOAT3(0.707f, 0.707f, 0.0f);
			vertice[idx].tex.x = j * du; 
			vertice[idx].tex.y = i * dv;
		}
	}
#pragma endregion

#pragma region define y

	std::unique_ptr<CShader> cShader(new CShader(device, L"HillCS.cso"));
	cShader->Apply(dContext);

	std::unique_ptr<ConstantBuffer<XMFLOAT2>> cbRange(new ConstantBuffer<XMFLOAT2>(device));
	cbRange->CSSetData(dContext, &heightRange,0);
	std::unique_ptr<ConstantBuffer<XMFLOAT2>> cbResol(new ConstantBuffer<XMFLOAT2>(device));
	cbResol->CSSetData(dContext, &XMFLOAT2(n, m), 1);

	dContext->CSSetShaderResources(0, 1, heightMap);

	Texture2D<float>* heightBuffer = new Texture2D<float>(
		device,
		CD3D11_TEXTURE2D_DESC(
			DXGI_FORMAT_R32_FLOAT,
			n, m,
			1, 1,
			D3D11_BIND_UNORDERED_ACCESS), 
		0);

	dContext->CSSetUnorderedAccessViews(0, 1, heightBuffer->UAV(device), nullptr);
	dContext->Dispatch(ceil(n/16.0f), ceil(m/16.0f), 1);
	dContext->CSSetUnorderedAccessViews(0, 1, heightBuffer->NullUAV(), nullptr);

	Texture2D<float>* outputBuffer = new Texture2D<float>(
		device,
		CD3D11_TEXTURE2D_DESC(
			DXGI_FORMAT_R32_FLOAT,
			n, m,
			1, 1,
			0,
			D3D11_USAGE_STAGING,
			D3D11_CPU_ACCESS_READ));

	dContext->CopyResource(outputBuffer->Get(), heightBuffer->Get());

	D3D11_MAPPED_SUBRESOURCE mapped;
	dContext->Map(outputBuffer->Get(), 0, D3D11_MAP_READ, 0, &mapped);
	float* outputArr = reinterpret_cast<float*>(mapped.pData);
	std::vector<float> testV;
	for (int y = 0; y < m; ++y)
	{
		for (int x = 0; x < n; ++x)
		{
			vertice[y * n + x].pos.y = *(((float*)mapped.pData + (y*int(mapped.RowPitch*0.25f)) + x));
			//vertice[y * n + x].pos.y = *((float*)((char*)mapped.pData + (y*mapped.RowPitch) + (x*sizeof(float)))); //possible either way
		}
	}
	dContext->Unmap(outputBuffer->Get(), 0);

#pragma endregion

#pragma region define index

	std::vector<UINT> indice((n - 1)*(m - 1) * 2 * 3);

	UINT k = 0; 
	for(UINT i = 0; i < m-1; ++i) 
	{ 
		for(UINT j = 0; j < n-1; ++j) 
		{ 
			indice[k] = i*n+j; 
			indice[k+1] = (i + 1)*n + j;
			indice[k+2] = i * n + j + 1;
			indice[k+3] = (i+1)*n+j; 
			indice[k+4] = (i + 1)*n + j + 1;
			indice[k+5] = i*n + j + 1;
			k += 6;
		}
	}
#pragma endregion


	Init(device, vertice.data(), vertice.size(), indice.data(), indice.size());
}


Hill::~Hill()
{
}

void Hill::Apply(ID3D11DeviceContext * dContext)
{
	Debugging::Draw("Normal of hill is only heading upward", 100, 30);

	Shape::Apply(dContext);
}
