#include "DCMScene.h"
#include "DynamicCubeMap.h"
#include "Sphere.h"
#include "Transform.h"
#include "Timer.h"
#include "Debugging.h"

DCMScene::DCMScene(IGraphic* graphic, Scene* captureScene)
	:Scene("DCM")
{
	timer = new Timer();

	dcmObj = new DynamicCubeMap(graphic, captureScene, new Sphere(4));
	dcmObj->transform->SetScale(20, 20, 20);
	dcmObj->transform->SetTranslation(60, 60, 0);
}

DCMScene::~DCMScene()
{
	delete timer;
	delete dcmObj;
}

void DCMScene::Logic_Update(Camera* camera)
{
	float elapsed = timer->Elapsed();
	const static XMFLOAT3 firstPos = XMFLOAT3(60, 60, 0);
	XMFLOAT3 pos = firstPos +
		XMFLOAT3(
		sin(elapsed * 0.3f) * 20,
		cos(elapsed * 0.4f) * 50,
		sin(elapsed * 0.2f) * 10
	);
	dcmObj->transform->SetTranslation(pos);
	Debugging::Instance()->Draw("Dynamic object pos = ", dcmObj->transform->GetPos(), 10, 10);
}

void DCMScene::Render_Update(Camera* camera)
{
	dcmObj->Update(camera);
}

void DCMScene::Render() const
{
	dcmObj->Render();
}
