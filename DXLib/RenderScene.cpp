
#include "pch.h"

#include "RenderScene.h"
#include "ObjectMgr.h"
#include "CameraMgr.h"
#include "Camera.h"
#include "Object.h"

using namespace DX;

void RenderScene::Update(float elapsed, float spf)
{
	for (auto obj : ObjectMgr::Instance()->Data())
	{
		for (auto cam  : CameraMgr::Instance()->Data())
		{
			// just for middle point checking
			auto oLayer = obj->Layer();
			auto cLayer = cam->Layer();

			if ((cam->Layer() & obj->Layer()) == obj->Layer())
			{
				XMMATRIX vp = cam->VMat() * cam->ProjMat();

				obj->Render(vp, cam->GetFrustum(), 0);
			}
		}
	}
}
