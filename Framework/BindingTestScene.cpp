#include "BindingTestScene.h"
#include "Cube.h"
#include "Shader.h"
#include "Transform.h"
#include "Camera.h"
#include "ShaderFormat.h"

BindingTestScene::BindingTestScene()
	:Scene("BindingTest")
{
	Camera* camera = new Camera("Binding", FRAME_KIND_PERSPECTIVE, NULL, NULL, 0.1, 1000, 1.1f, 1.0f);
	camera->SetPos(XMFLOAT3(0, 0, -100));
	camera->SetRot(FORWARD, UP);
	camera->SetMain();

	obj = new Object(
		new Cube(), 
		"BindingTestVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts),
		"","","",
		"",
		2);
	obj2 = new Object(
		new Cube(),
		"BindingTest2VS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts),
		"", "", "",
		"",
		2);
	obj->vs->AddCB(0, 1, sizeof(XMMATRIX));
	obj->vs->AddCB(1, 1, sizeof(XMFLOAT4));
	obj->ps->AddCB(0, 1, sizeof(XMMATRIX));
	obj->ps->AddCB(1, 1, sizeof(XMFLOAT4));
}

void BindingTestScene::Logic_Update()
{
	
}

void BindingTestScene::Render_Update(const Camera* camera, float elapsed, float spf)
{
	if (bb1)
	{
		obj->vs->RemoveCB(0);
		obj->vs->RemoveCB(1);
		obj->ps->RemoveCB(0);
		//obj->ps->RemoveCB(1);
		bb1 = false;
	}
	if (!b1)
	{
		XMFLOAT4 data = XMFLOAT4(5, 5, 5, 5);
		obj->vs->WriteCB(0, &(obj->transform->WorldMatrix()*camera->VPMat(2)));
		obj->vs->WriteCB(1, &data);
		obj->ps->WriteCB(0, &(obj->transform->WorldMatrix() * camera->VPMat(2)));
		//obj->ps->WriteCB(1, &data);

		b1 = true;
		bb1 = true;
	}

	//obj->vs->RemoveCB(0);
	//obj2->Render_Shader();

}

void BindingTestScene::Render() const
{
	obj->Render();
	//obj2->Render();
}
