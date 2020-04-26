#include "stdafx.h"
#include "SoundMgr.h"

SoundMgr::SoundMgr()
{

	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	audEngine = std::make_unique<AudioEngine>(eflags);
	
}

void SoundMgr::Update()
{
	if (!audEngine->Update())
		assert(false && "WTF in audio device");
}

void SoundMgr::Add(std::string key, std::wstring path)
{
	if (audios.count(key))
		return;

	const wchar_t* wc = path.c_str();
	auto curSE = new SoundEffect(audEngine.get(), path.c_str());
	audios.insert(std::pair<std::string, std::unique_ptr<SoundEffect>>(key, std::move(curSE)));
}

void SoundMgr::Play(std::string key)
{
	if (!audios.count(key))
		return;

	audios[key]->Play();
}
