#include "BindingTestScene.h"
#include "Cube.h"
#include "Shader.h"

BindingTestScene::BindingTestScene()
	:Scene("BindingTest")
{
	obj = new Object(
		new Cube(), 
		"BindingTestVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts),
		"","","",
		"BindingTestPS.cso",
		2);
	obj2 = new Object(
		new Cube(),
		"BindingTest2VS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts),
		"", "", "",
		"",
		2);
	obj->vs->AddCB(0, 1, sizeof(XMFLOAT4));
}

void BindingTestScene::Logic_Update()
{
}

void BindingTestScene::Render_Update(const Camera* camera, float elapsed)
{
	if (!isOnce)
	{
		XMFLOAT4 data = XMFLOAT4(5, 5, 5, 5);
		obj->vs->WriteCB(0, &data);

		isOnce = true;
	}
	obj->vs->RemoveCB(0);
	//obj2->Render_Shader();

}

void BindingTestScene::Render() const
{
	obj->Render();
	//obj2->Render_Geom();
}
