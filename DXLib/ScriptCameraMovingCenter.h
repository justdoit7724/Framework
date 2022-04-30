#pragma once
#include "Script.h"
class ScriptCameraMovingCenter : public SubScript
{
public:
    ScriptCameraMovingCenter(Script* owner);

    void Update() override;
};

