#pragma once
#include "Asset.h"
class Texture :
    public Asset
{
public:
    void SetKey(std::string path);
    std::string GetKey();
private:
    //total path
    std::string m_path;
};

