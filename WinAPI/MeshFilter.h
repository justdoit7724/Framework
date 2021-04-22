#pragma once

#include "Component.h"

class Mesh;

class MeshFilter : public Component
{
public:
	MeshFilter(Object* owner);

	void Update(DX::Graphic* graphic) override;
	void Render(DX::Graphic* graphic)const override;

	void GetLBound(OUT Geom::Bound* bound);
	void SetMesh(std::string key);

private:

	std::string m_meshKey;
	Geom::Bound m_outBound;

};

