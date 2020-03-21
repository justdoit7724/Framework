#pragma once

class Terrain
{
public:
	Terrain();

	void LoadHeightMap(std::string fileAddress, XMINT2 sizeCount, float heightScale);

private:

	struct TerrainVertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
		XMFLOAT2 boundsY;
	};

	bool IsInBound(int x, int y);
	float Average(int x, int y);
	void Smooth();
	void BuildSRV();
	float GetWidth() const;
	float GetDepth() const;
	void BuildQuadPatchVB();
	void BuildQuadPatchIB();
	void CalcAllPatchBoundsY();

	std::vector<float> heightMap;
	XMINT2 sizeCount;
	float cellSpacing;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;

	const int cellCountPerPatch = 64;
	int numPatchVertRow;
	int numPatchVertCol;
	int numPatchVert;
	int numPatchQuad;
	std::vector<XMFLOAT2> patchBoundsY;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vb;
	Microsoft::WRL::ComPtr<ID3D11Buffer> ib;
};