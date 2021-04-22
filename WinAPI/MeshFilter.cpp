
#include "stdafx.h"

#include "MeshFilter.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "MeshRenderer.h"
#include "ResourceMgr.h"
#include "Object.h"


MeshFilter::MeshFilter(Object* owner)
	:
	Component(CMPNT_KIND::MeshFilter, owner),
	m_meshKey("")
{
}



void CalculateTangent(XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT2 t0, XMFLOAT2 t1, XMFLOAT2 t2, OUT XMFLOAT3* tangent)
{
	XMFLOAT2 dt0 = t1 - t0;
	XMFLOAT2 dt1 = t2 - t0;
	XMFLOAT3 dv0 = v1 - v0;
	XMFLOAT3 dv1 = v2 - v0;
	float determinant = dt0.x * dt1.y - dt0.y * dt1.x;
	*tangent = dv0 * (dt1.y / determinant) - dv1 * (dt0.y / determinant);
}

void MeshFilter::GetLBound(OUT Geom::Bound* bound)
{
	*bound = m_outBound;
}

void MeshFilter::SetMesh(std::string key)
{
	m_meshKey = key;
}

void MeshFilter::Update(DX::Graphic* graphic){

}
void MeshFilter::Render(DX::Graphic* graphic)const 
{
	Mesh* mesh = SGL_ResMgr.GetMesh(m_meshKey);
	MeshRenderer* renderer = (MeshRenderer*)m_object->GetComponentFromParent(CMPNT_KIND::MeshRenderer);

	const std::vector<SubMesh*>& subMeshes = mesh->GetSubMesh();
	for (int i=0; i< subMeshes.size(); ++i)
	{
		std::string matKey = renderer->GetMaterial(i);
		Material* curMat = SGL_ResMgr.GetMaterial(graphic, matKey);
		std::string curDiffKey = curMat->GetDiff();
		std::string curNormKey = curMat->GetNorm();
		Texture* curDiff = SGL_ResMgr.GetTexture(graphic, curDiffKey);
		Texture* curNorm = SGL_ResMgr.GetTexture(graphic, curNormKey);
		curDiffKey = curDiff->GetKey();
		curNormKey = curNorm->GetKey();

		std::vector<const void*> datas;
		datas.push_back(&curDiffKey);
		datas.push_back(&curNormKey);
		graphic->SetPipelineData(PIP_REG::PIXEL_TEX, datas);

		datas.clear();
		datas.push_back(&subMeshes[i]->m_key);
		graphic->SetPipelineData(PIP_REG::TRANSFORM_MESH, datas);
	}
}
