#include "Scene.h"
#include "SceneMgr.h"
#include "Object.h"
#include "Shader.h"
#include "ShaderFormat.h"

Scene::Scene(std::string key)
	:key(key)
{
	shadowMapVS = new VShader("ShadowMapVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts));
	shadowMapVS->AddCB(0, 1, sizeof(XMMATRIX));
}

Scene::~Scene()
{
	delete shadowMapVS;

	SceneMgr::Instance()->Remove(key);
}

void Scene::Render_Update(const Camera* camera, float elapsed)
{
	for (auto obj : objs)
	{
		obj->Update(camera, elapsed, XMMatrixIdentity());
	}
}

void Scene::Render() const
{
	for (auto obj : objs)
	{
		obj->Render();
	}
}
