#include "stdafx.h"
#include "Terrain.h"

Terrain::Terrain()
{
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

	data.resize(sizeCount.x * sizeCount.y, 0);
	for (int i = 0; i < sizeCount.x * sizeCount.y; ++i)
	{
		data[i] = (in[i] / 255) * heightScale;
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
				total += data[i + j * sizeCount.x];
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

			data[idx] = Average(x, y);
		}
	}
}
