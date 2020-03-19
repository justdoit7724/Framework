#pragma once

class Terrain
{
public:
	Terrain();

	void LoadHeightMap(std::string fileAddress, XMINT2 sizeCount, float heightScale);

private:

	bool IsInBound(int x, int y);
	float Average(int x, int y);
	void Smooth();

	std::vector<float> data;
	XMINT2 sizeCount;
};