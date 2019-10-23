#include "DCMScene.h"
#include "DynamicCubeMap.h"
#include "Sphere.h"
#include "Transform.h"
#include "Timer.h"
#include "Mirror.h"

DCMScene::DCMScene(IGraphic* graphic, Scene* captureScene)
	:Scene("DCM")
{
	timer = new Timer();

	dcmObj = new DynamicCubeMap(captureScene, new Sphere(4));
	dcmObj->transform->SetScale(40, 40, 40);
	dcmObj->transform->SetTranslation(0, 30, 0);
	AddObj(dcmObj);

	/*mirror = new Mirror(captureScene, 1024, 512);
	mirror->transform->SetScale(190, 90, 1);
	mirror->transform->SetTranslation(0, 90, 0);
	AddObj(mirror);*/
}

DCMScene::~DCMScene()
{
	delete timer;
}

void DCMScene::Update(float elapsed, float spf)
{
}
