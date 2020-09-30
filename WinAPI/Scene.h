#pragma once

class SceneMgr;

class Scene
{
public:

	virtual void Update(float elapsed, float spf) = 0;

	virtual void Message(UINT msg) {};


private:
	friend class SceneMgr;

	const wchar_t* key;
};


