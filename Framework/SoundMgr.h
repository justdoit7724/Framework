

#pragma once
#include "DX_info.h"
#include "Singleton.h"


#include <directxtk_desktop_2015.2019.12.17.1/include/Audio.h>

class SoundMgr : public Singleton<SoundMgr>
{
public:
	SoundMgr();

	void Update();
	void Add(std::string key, std::wstring path);
	void Play(std::string key);

private:
	std::unique_ptr<AudioEngine> audEngine;
	std::unordered_map<std::string,std::unique_ptr<SoundEffect>> audios;
};