#pragma once

#include "DefComponent.h"

class Component;

class  Object
{
public:
	Object();
	virtual ~Object();

	virtual void Update(DX::Graphic* graphic);
	virtual void Render(DX::Graphic* graphic) const;




	//Camera related----------------------------------
public:
	virtual bool IsPicking(Geom::Ray ray)const;
	virtual void UpdateBound();
	bool IsInsideFrustum(const Frustum* frustum) const;
	Geom::Sphere GetOccludeBound();
	int Layer()const;
	void SetLayer(int l);
private:
	Geom::Sphere m_occludeBound;
	int layer;


	//Component & Hierarchy-------------------------------
public:
	void AddComponent(CMPNT_KIND key);
	void RemoveComponent(CMPNT_KIND key, int id=0);
	Component* GetComponent(CMPNT_KIND key, int id=0);
	Component* GetComponentFromParent(CMPNT_KIND key, int id = 0);
	Component* GetComponentFromChildren(CMPNT_KIND key, int id = 0);
private:
	BOOL HasComponent(CMPNT_KIND key, int id =-1);
	void AddChild(Object* obj);
	void RemoveChild(std::string name);
	void RemoveChild(int index);
	void RemoveChild(Object* obj);
	BOOL HasChild(Object* obj);
	void SetParent(Object* parent);
	std::map< CMPNT_KIND, std::vector<Component*>> m_mComponent;
	std::vector<Object*> m_vChild;
	Object* m_parent;

	//ID-----------------------------------------------
public:
	void SetName(std::string name);
	std::string GetName();
private:
	std::string m_name;


	// Flag
public:
	void SetEnabled(bool e) { enabled = e; }
private:
	bool enabled = true;

};


