#include "DCMScene.h"
#include "DynamicCubeMap.h"
#include "Sphere.h"
#include "Transform.h"
#include "Timer.h"
#include "Debugging.h"
#include "Mirror.h"

DCMScene::DCMScene(IGraphic* graphic, Scene* captureScene)
	:Scene("DCM")
{
	timer = new Timer();

	dcmObj = new DynamicCubeMap(graphic, captureScene, new Sphere(4));
	dcmObj->transform->SetScale(20, 20, 20);
	dcmObj->transform->SetTranslation(60, 60, 0);

	mirror = new Mirror(captureScene, 512, 512);
	mirror->transform->SetScale(10, 10, 1);
}

DCMScene::~DCMScene()
{
	delete timer;
	delete dcmObj;
	delete mirror;
}

void DCMScene::Logic_Update(Camera* camera)
{
	float elapsed = timer->Elapsed();
	const static XMFLOAT3 firstPos = XMFLOAT3(60, 60, 0);
	XMFLOAT3 pos = firstPos +
		XMFLOAT3(
		sin(elapsed * 0.2f) * 10,
		cos(elapsed * 0.3f) * 50,
		sin(elapsed * 0.1f) * 15
	);
	dcmObj->transform->SetTranslation(pos);
	mirror->Update(camera);
	dcmObj->Update(camera);
}

void DCMScene::Render_Update(Camera* camera)
{
}

void DCMScene::Render() const
{
	dcmObj->Render();
	mirror->Render();
}
