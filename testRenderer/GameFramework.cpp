#include "stdafx.h"
#include "GameFramework.h"
#include "Player.h"

CGameFramework::CGameFramework()
	: m_Active{true}
	, m_hInstance{nullptr}
	, m_hWnd{ nullptr }
	, m_rcClient{}
	, m_hDCFrameBuffer{ nullptr }
	, m_hBitmapFrmaeBuffer{ nullptr }
	, m_hBitmapSelect{ nullptr }
	, m_OldCursorPos{}
	, m_pszFrameRate{}
	, m_keyDown{}
	, m_pScene{ nullptr }
	, m_lockOnObejct{ nullptr }
	, m_lockOnObjectOriginColor{ RGB(0,0,0) }
{
}

CGameFramework::~CGameFramework()
{
}

void CGameFramework::CreateGame(HINSTANCE hInstance, HWND hWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hWnd;

	GetClientRect(m_hWnd, &m_rcClient);

	HDC hDC = GetDC(m_hWnd);
	m_hDCFrameBuffer = CreateCompatibleDC(hDC);
	m_hBitmapFrmaeBuffer = CreateCompatibleBitmap(hDC, m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top);
	SelectObject(m_hDCFrameBuffer, m_hBitmapFrmaeBuffer);

	ReleaseDC(m_hWnd, hDC);
	SetBkMode(m_hDCFrameBuffer, TRANSPARENT);

	CreateObjects();

	_tcscpy_s(m_pszFrameRate, _T("LabProject ("));
}

void CGameFramework::DestroyGame()
{
	DestroyObjects();

	if (m_hBitmapFrmaeBuffer)
	{
		DeleteObject(m_hBitmapFrmaeBuffer);
	}
	if (m_hDCFrameBuffer)
	{
		DeleteDC(m_hDCFrameBuffer);
	}
	
	m_lockOnObejct.release();
	
}

void CGameFramework::ClearFrameBuffer(DWORD color)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 0, color);
	HPEN hOldPen = (HPEN)::SelectObject(m_hDCFrameBuffer, hPen);
	HBRUSH hBrush = ::CreateSolidBrush(color);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hDCFrameBuffer, hBrush);
	::Rectangle(m_hDCFrameBuffer, m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom);
	::SelectObject(m_hDCFrameBuffer, hOldBrush);
	::SelectObject(m_hDCFrameBuffer, hOldPen);
	::DeleteObject(hPen);
	::DeleteObject(hBrush);
}

void CGameFramework::CopyFrameBuffer()
{
	HDC hDC = GetDC(m_hWnd);
	BitBlt(hDC, m_rcClient.left, m_rcClient.top, m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top, m_hDCFrameBuffer, m_rcClient.left, m_rcClient.top, SRCCOPY);
	ReleaseDC(m_hWnd, hDC);
}

void CGameFramework::CreateObjects()
{
	m_pScene = std::make_unique<CScene>();
	m_pScene->CreateScene();
}

void CGameFramework::DestroyObjects()
{
}

void CGameFramework::HandleInput()
{
	if (GetCapture() == m_hWnd)
	{
		SetCursor(nullptr);
		POINT ptCursorPos;
		GetCursorPos(&ptCursorPos);
		float cxMouseDelta = (float)(ptCursorPos.x - m_OldCursorPos.x) / 3.0f;
		float cyMouseDelta = (float)(ptCursorPos.y - m_OldCursorPos.y) / 3.0f;
		SetCursorPos(m_OldCursorPos.x, m_OldCursorPos.y);
		if (cxMouseDelta || cyMouseDelta)
		{
			if(GetAsyncKeyState(VK_LBUTTON) & 0x8000) m_pScene->GetPlayer()->AddRotationAngle(cyMouseDelta, cxMouseDelta, 0.0f);
			//if(GetAsyncKeyState(VK_RBUTTON) & 0x8000) m_pScene->GetPlayer()->SetRotationAngle(cyMouseDelta, 0.0f, -cxMouseDelta);
		}
	}

	m_keyDown = 0;

	if (GetAsyncKeyState(VK_W) & 0x8000) m_keyDown |= DIR_FORWARD;
	if (GetAsyncKeyState(VK_S) & 0x8000) m_keyDown |= DIR_BACKWARD;
	if (GetAsyncKeyState(VK_D) & 0x8000) m_keyDown |= DIR_RIGHT;
	if (GetAsyncKeyState(VK_A) & 0x8000) m_keyDown |= DIR_LEFT;
	if (GetAsyncKeyState(VK_Q) & 0x8000) m_keyDown |= DIR_UP;
	if (GetAsyncKeyState(VK_E) & 0x8000) m_keyDown |= DIR_DOWN;

	if (GetAsyncKeyState(VK_SPACE) & 0x8000) (static_cast<CTankPlayer*>((m_pScene->GetPlayer())))->FireBullet();

	m_pScene->HandleInput(m_keyDown);
}

void CGameFramework::UpdateObjects()
{
	m_pScene->Update((float)m_gameTimer.GetDeltaTime());
}

void CGameFramework::CollideObjects()
{
	m_pScene->Collide();
}

void CGameFramework::RenderObejcts()
{
	ClearFrameBuffer(RGB(0, 0, 0));

	m_pScene->Render(m_hDCFrameBuffer);

	CopyFrameBuffer();
	SetWindowText(m_hWnd, m_pszFrameRate);
}

void CGameFramework::FrameAdvance()
{
	m_gameTimer.Tick(60.0f);

	HandleInput();

	UpdateObjects();

	CollideObjects();

	RenderObejcts();

	m_gameTimer.GetFrameRate(m_pszFrameRate + 12, 37);
	::SetWindowText(m_hWnd, m_pszFrameRate);
}

void CGameFramework::OnWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnMouseMessage(hWnd, message, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnKeyBoardMessage(hWnd, message, wParam, lParam);
		break;
	}
}

void CGameFramework::OnKeyBoardMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		case VK_RETURN:
			break;
		case VK_CONTROL:
			break;
		default:
			//m_pScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
			break;
		}
		break;
	default:
		break;
	}
}

void CGameFramework::OnMouseMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
		SetCapture(hWnd);
		GetCursorPos(&m_OldCursorPos);
		if (message == WM_RBUTTONDOWN)
		{
			if (m_lockOnObejct)
			{
				m_lockOnObejct->SetColor(m_lockOnObjectOriginColor);
				m_lockOnObejct.release();
			}
			m_lockOnObejct.reset(m_pScene->GetPickedObject(LOWORD(lParam), HIWORD(lParam)));
			if (m_lockOnObejct)
			{
				m_lockOnObjectOriginColor = m_lockOnObejct->SetColor(RGB(255, 0, 0));
			}
		}
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}



