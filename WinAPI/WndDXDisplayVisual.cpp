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
}

WndDXDisplayVisual::~WndDXDisplayVisual()
{
}

void WndDXDisplayVisual::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_SHOWWINDOW:

		SAFEDELETE(g_dxGraphic);
		g_dxGraphic = new DX::Graphic(m_hWnd);

		m_scene = new VisualAAScene(g_dxGraphic->Device(), g_dxGraphic->DContext(), L"VisualDisplay");
		m_scene->WM_Resize(GetWidth(), GetHeight());
		break;
		case WM_COMMAND:

			switch (LOWORD(wparam))
			{
				case ID_COMMAND_REALTIME_UPDATE:
				{
					Timer* timer = (Timer*)lparam;

					m_scene->Update(timer->Elapsed(), timer->SPF());
					g_dxGraphic->Present();
				}
				break;
				case ID_CONTROL_RESOLUTION:
			
					m_iMSAA = HIWORD(wparam);

					SAFEDELETE(g_dxGraphic);
					g_dxGraphic = new DX::Graphic(m_hWnd);

					m_scene = new VisualAAScene(g_dxGraphic->Device(), g_dxGraphic->DContext(), L"VisualDisplay");
					m_scene->WM_Resize(GetWidth(), GetHeight());

					break;
			}
		break;
	}

}
