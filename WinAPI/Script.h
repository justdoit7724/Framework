#pragma once
#include "Component.h"


class Script;

class SubScript
{
public:
    // factory pattern
    static SubScript* Create(SCRIPT_KIND kind, Script* owner);

    virtual void Update() = 0;

    const Script* m_script;

protected:
    SubScript(Script* owner);

};

class Script :
    public Component
{
    Script(Object* owner);
    ~Script();

    void Update(DX::Graphic* graphic)override;
    void Render(DX::Graphic* graphic)const override;

    void Select(SCRIPT_KIND kind);

private:
    SubScript* m_subScript;
};

