#pragma once
#include "Component.h"

class Material;

class MeshRenderer : public Component
{
	public:
		MeshRenderer(Object* owner);
		~MeshRenderer();

		void Update(DX::Graphic* graphic) override;
		void Render(DX::Graphic* graphic)const override;

		void SetMaterial(UINT id, std::string key);
		std::string GetMaterial(UINT id);



	private:

		std::vector<std::string> m_matKeys;


};

