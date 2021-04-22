#pragma once
#include "Asset.h"
class Shader :
    public Asset
{
public:

    void SetShader(std::string key);

protected:
    std::string m_key;
};

class VShader : public Shader
{

};
class HShader : public Shader
{

};
class DShader : public Shader
{

};
class GShader : public Shader
{

};
class PShader : public Shader
{

};
