#include "stdafx.h"
#include "RenderScene.h"
#include "ObjectMgr.h"
#include "CameraMgr.h"
#include "Camera.h"
#include "Object.h"

void RenderScene::Update(float elapsed, float spf)
{
	for (auto obj : ObjectMgr::Instance()->Data())
	{
		for (auto cam  : CameraMgr::Instance()->Data())
		{
			if ((cam->Layer() & obj->Layer()) == obj->Layer())
			{
				XMMATRIX vp = cam->VMat() * cam->ProjMat();

				obj->Render(vp, cam->GetFrustum(), 0);
			}
		}
	}
}
