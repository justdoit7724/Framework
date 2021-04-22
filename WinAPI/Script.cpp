#include "stdafx.h"
#include "Script.h"
#include "ScriptCameraMovingCenter.h"

SubScript* SubScript::Create(SCRIPT_KIND kind, Script* owner)
{
	switch (kind)
	{
	case SCRIPT_KIND::CameraMovingCenter:
		return new ScriptCameraMovingCenter(owner);
	}

	return nullptr;
}

SubScript::SubScript(Script* script)
	:m_script(script)
{
}


Script::Script(Object* owner)
	:Component(CMPNT_KIND::Script, owner),
	m_subScript(nullptr)
{
}

Script::~Script()
{
	if (m_subScript)
		delete m_subScript;
}

void Script::Update(DX::Graphic* graphic)
{
	if (m_subScript)
		m_subScript->Update();
}

void Script::Render(DX::Graphic* graphic)const
{
}

void Script::Select(SCRIPT_KIND kind)
{
	if (m_subScript) delete m_subScript;

	m_subScript = SubScript::Create(kind, this);
}

