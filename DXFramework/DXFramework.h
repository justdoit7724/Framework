#pragma once
#include <wtypes.h>

class Graphic;
class Timer;
class PlayScene;

/*
사용자용 인터페이스 클래스
해당 클래스에 있는 public함수만 사용하여서,  
내부 동작 로직을 구현할 필요없이 간편히
원하는 DX그래픽 효과 구현
*/
class DXFramework
{
public:
	// 초기화
	DXFramework(HWND hwnd, int scnWidth, int scnHeight);

	// 업데이트 루프
	void Update();

	// 인풋 함수
	void OnWndProc(UINT msg, WPARAM wparam, LPARAM lparam);

	void CreateObject(const char* filename);

private:

	Graphic* env = nullptr;
	Timer* timer = nullptr;

	PlayScene* playScene = nullptr;

};
