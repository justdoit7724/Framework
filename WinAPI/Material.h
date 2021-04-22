#pragma once
#include "Asset.h"

class Texture;

class Material :
    public Asset
{
public:
    void SetDiff(std::string diff);
    void SetNorm(std::string norm);
    std::string GetDiff();
    std::string GetNorm();

private:
    std::string m_diff;
    std::string m_norm;
};

