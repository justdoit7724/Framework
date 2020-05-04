#include "DXFramework.h"
#include "DX_info.h"
#include "Graphic.h"
#include "Timer.h"
#include "SceneMgr.h"
#include "PlayScene.h"
#include "RenderScene.h"
#include "DebuggingScene.h"
#include "Debugging.h"
#include "Keyboard.h"
#include "Mouse.h"

DXFramework::DXFramework(HWND hwnd, int scnWidth, int scnHeight)
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    env = new Graphic(hwnd, scnWidth, scnHeight);
    timer = new Timer();

	playScene = new PlayScene();
    SceneMgr::Instance()->Add("Play", playScene);
    SceneMgr::Instance()->Add("Render", new RenderScene());
    SceneMgr::Instance()->Add("Debug", new DebuggingScene());
}

void DXFramework::Update()
{
    SceneMgr::Instance()->Process(timer->Elapsed(), timer->SPF());

    Mouse::Instance()->Update(timer->SPF());

    env->Present();

    timer->Update();

    Keyboard::Update();
}



void DXFramework::OnWndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_KEYUP:
        Keyboard::Release(static_cast<char>(wparam));
        break;
    case WM_KEYDOWN:
        Keyboard::Press(wparam);
        break;
    case WM_LBUTTONDOWN:
        Mouse::Instance()->UpdateLeft(true);
        break;
    case WM_LBUTTONUP:
        Mouse::Instance()->UpdateLeft(false);
        break;
    case WM_RBUTTONDOWN:
        Mouse::Instance()->UpdateRight(true);
        break;
    case WM_RBUTTONUP:
        Mouse::Instance()->UpdateRight(false);
        break;
    case WM_MOUSEMOVE:
        Mouse::Instance()->UpdatePt(lparam);
        break;
    case WM_MOUSEWHEEL:
        Mouse::Instance()->StartWheel(GET_WHEEL_DELTA_WPARAM(wparam) > 0);
        break;
    default:
        break;
    }
}

void DXFramework::CreateObject(const char* filename)
{
	playScene->CreateModel(filename);
	playScene->CreateGrid();
}
