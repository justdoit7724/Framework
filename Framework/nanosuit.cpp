#include "nanosuit.h"
#include "MeshLoader.h"
#include "Transform.h"

nanosuit::nanosuit()
{
	//debug
	MeshLoader::Load(objs, "nanosuit", "nanosuit.obj");
}

void nanosuit::Update()
{
	for (auto obj : objs)
	{
		obj->Update();
	}
}

void nanosuit::Render(const Camera* camera, UINT sceneDepth) const
{
	for (auto mesh : objs)
	{
		mesh->Render(camera, sceneDepth);
	}
}

void nanosuit::SetRot(XMFLOAT3 rot)
{
	for (auto mesh : objs)
	{
		mesh->transform->SetRot(rot);
	}
}

void nanosuit::SetPos(XMFLOAT3 pos)
{
	for (auto mesh : objs)
	{
		mesh->transform->SetTranslation(pos);
	}
}

void nanosuit::SetScale(XMFLOAT3 scale)
{
	for (auto mesh : objs)
	{
		mesh->transform->SetScale(scale);
	}
}
