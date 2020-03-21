#include "stdafx.h"
#include "Terrain.h"
#include "DX_info.h"

Terrain::Terrain()
{
	numPatchVertRow = (sizeCount.y - 1) / cellCountPerPatch + 1;
	numPatchVertCol = (sizeCount.x - 1) / cellCountPerPatch + 1;
	numPatchVert = numPatchVertRow * numPatchVertCol;
	numPatchQuad = (numPatchVertRow - 1) * (numPatchVertCol - 1);
}

void Terrain::CalcAllPatchBoundsY()
{
	patchBoundsY.resize(numPatchQuad);

	for (int i = 0; i < numPatchVertRow - 1; ++i)
	{
		for (int j = 0; j < numPatchVertCol - 1; ++i)
		{
			int	x0 = j * cellCountPerPatch;
			int x1 = (j + 1) * cellCountPerPatch;
			int y0 = i * cellCountPerPatch;
			int y1 = (i + 1) * cellCountPerPatch;

			float minY = FLT_MAX;
			float maxY = -FLT_MAX;
			for (int y = y0; y <= y1; ++y)
			{
				for (int x = x0; x < x1; ++x)
				{
					int k = y * sizeCount.x + x;
					minY = fminf(minY, heightMap[k]);
					maxY = fmaxf(maxY, heightMap[k]);
				}
			}

			int patchID = i * (numPatchVertCol - 1) + j;
			patchBoundsY[patchID] = XMFLOAT2(minY, maxY);
		}
	}
}

void Terrain::LoadHeightMap(std::string fileAddress, XMINT2 sizeCount, float heightScale)
{
	this->sizeCount = sizeCount;

	std::vector<unsigned char> in(sizeCount.x * sizeCount.y);

	std::ifstream inFile(fileAddress, std::ios_base::binary);
	
	if (inFile)
	{
		inFile.read((char*) & in[0], (long long)in.size());
		inFile.close();
	}

	heightMap.resize(sizeCount.x * sizeCount.y, 0);
	for (int i = 0; i < sizeCount.x * sizeCount.y; ++i)
	{
		heightMap[i] = (in[i] / 255) * heightScale;
	}
}

bool Terrain::IsInBound(int x, int y)
{
	return (
		x>0 &&
		y>0 &&
		x<sizeCount.x-1 &&
		y<sizeCount.y-1);
}

float Terrain::Average(int x, int y)
{
	float weight = 0;
	float total = 0;
	for (int i = x - 1; i <= x + 1; ++i)
	{
		for (int j = y - 1; j <= y + 1; ++j)
		{
			if (IsInBound(i, j))
			{
				total += heightMap[i + j * sizeCount.x];
				weight += 1;
			}
		}
	}

	return (total/weight);
}

void Terrain::Smooth()
{
	for (int y = 0; y < sizeCount.y; ++y)
	{
		for (int x = 0; x < sizeCount.x; ++x)
		{
			int idx = x + y * sizeCount.x;

			heightMap[idx] = Average(x, y);
		}
	}
}

void Terrain::BuildSRV()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = sizeCount.x;
	texDesc.Height = sizeCount.y;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = &heightMap[0];
	subData.SysMemPitch = sizeof(float) * sizeCount.x;
	subData.SysMemSlicePitch = 0;

	ComPtr<ID3D11Texture2D> tex = nullptr;
	r_assert(
		DX_Device->CreateTexture2D(&texDesc, &subData, tex.GetAddressOf())
	);
	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	r_assert(
		DX_Device->CreateShaderResourceView(tex.Get(), &srvDesc, srv.GetAddressOf())
	);
}

float Terrain::GetWidth() const
{
	return (sizeCount.x * cellSpacing);
}

float Terrain::GetDepth() const
{
	return (sizeCount.y * cellSpacing);
}

void Terrain::BuildQuadPatchVB()
{
	std::vector<TerrainVertex> patchVertice(numPatchVertRow * numPatchVertCol);

	float hWidth = 0.5f * GetWidth();
	float hDepth = 0.5f * GetDepth();

	float patchWidth = GetWidth() / (numPatchVertCol - 1);
	float patchDepth = GetDepth() / (numPatchVertRow - 1);
	float du = 1.0f / (numPatchVertCol - 1);
	float dv = 1.0f / (numPatchVertRow - 1);

	for (int i = 0; i < numPatchVertRow; ++i)
	{
		float z = hDepth - i * patchDepth;
		for (int j = 0; j < numPatchVertCol; ++j)
		{
			float x = -hWidth + j * patchWidth;
			int idx = i * numPatchVertCol + j;
			patchVertice[idx].pos = XMFLOAT3(x, 0.0f, z);
			patchVertice[idx].tex = XMFLOAT2(j * du, i * dv);

			int patchID = i * (numPatchVertCol - 1) + j;
			patchVertice[idx].boundsY = patchBoundsY[patchID];
		}
	}

	D3D11_BUFFER_DESC vbDesc;
	vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vbDesc.ByteWidth = sizeof(TerrainVertex) * patchVertice.size();
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = &patchVertice[0];
	r_assert(
		DX_Device->CreateBuffer(&vbDesc, &subData, vb.GetAddressOf())
	);
}

void Terrain::BuildQuadPatchIB()
{
	std::vector<unsigned short> indice(numPatchQuad * 4);

	int k = 0;
	for (int i = 0; i < numPatchVertRow - 1; ++i)
	{
		for (int j = 0; j < numPatchVertCol - 1; ++j)
		{
			indice[k++] = i * numPatchVertCol + j;
			indice[k++] = i * numPatchVertCol + j + 1;

			indice[k++] = (i + 1) * numPatchVertCol + j;
			indice[k++] = (i + 1) * numPatchVertCol + j + 1;
		}
	}

	D3D11_BUFFER_DESC ibDesc;
	ibDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ibDesc.ByteWidth = sizeof(unsigned short) * indice.size();
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = &indice[0];
	r_assert(
		DX_Device->CreateBuffer(&ibDesc, &subData, ib.GetAddressOf())
	);
}
