#include "stdafx.h"
#include "WndDXDisplayVisual.h"
#include "VisualAAScene.h"
#include "Timer.h"

WndDXDisplayVisual::WndDXDisplayVisual(HINSTANCE hInstance, HWND parent, int x, int y, int width, int height)
	:Window(hInstance, L"DX display"), m_iMSAA(1)
{
	m_hWnd = CreateWindowEx(
		0,
		m_wstrName.c_str(),
		m_wstrName.c_str(),
		(DWORD)WindowType::Popup,
		x, y,
		width, height,
		parent,
		NULL,
		hInstance,
		nullptr);
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	m_graphic = new DX::Graphic(m_hWnd);
	m_scene = new VisualAAScene(m_graphic->Device(), m_graphic->DContext(), L"VisualDisplay");
	m_scene->WM_Resize(width, height);
}

WndDXDisplayVisual::~WndDXDisplayVisual()
{
	delete m_scene;
	delete m_graphic;
	DestroyWindow(m_hWnd);
}

void WndDXDisplayVisual::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_COMMAND:

			switch (LOWORD(wparam))
			{
				case ID_COMMAND_REALTIME_UPDATE:
				{
					Timer* timer = (Timer*)lparam;

					m_scene->Update(timer->Elapsed(), timer->SPF());
					m_graphic->Present();
				}
				break;
				case ID_CONTROL_RESOLUTION:
			
					m_scene->SetResolution(HIWORD(wparam));
					break;
			}
		break;

		case WM_KEYDOWN:

			switch (wparam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				return;
			}
		break;
	}

}
