#include "stdafx.h"
#include "GameFramework.h"

CGameFramework::CGameFramework()
	: m_Active{true}
	, m_hInstance{nullptr}
	, m_hWnd{ nullptr }
	, m_rcClient{}
	, m_hDCFrameBuffer{ nullptr }
	, m_hBitmapFrmaeBuffer{ nullptr }
	, m_hBitmapSelect{ nullptr }
	, m_ptOldCursorPos{}
	, m_pszFrameRate{}
	, m_keyDown{}
	, m_pScene{ nullptr }
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
	//static UCHAR keyBuffer[256];
	{
		m_keyDown = 0;
		
		//if (keyBuffer[0x57] & 0xF0) m_keyDown |= DIR_FORWARD;
		//if (keyBuffer[0x53] & 0xF0)	m_keyDown |= DIR_BACKWARD;
		//if (keyBuffer[0x44] & 0xF0)	m_keyDown |= DIR_RIGHT;
		//if (keyBuffer[0x41] & 0xF0)	m_keyDown |= DIR_LEFT;
		//if (keyBuffer[0x51] & 0xF0)	m_keyDown |= DIR_UP;
		//if (keyBuffer[0x45] & 0xF0)	m_keyDown |= DIR_DOWN;

		if (GetAsyncKeyState(VK_W) & 0x8000) m_keyDown |= DIR_FORWARD;
		if (GetAsyncKeyState(VK_S) & 0x8000) m_keyDown |= DIR_BACKWARD;
		if (GetAsyncKeyState(VK_D) & 0x8000) m_keyDown |= DIR_RIGHT;
		if (GetAsyncKeyState(VK_A) & 0x8000) m_keyDown |= DIR_LEFT;
		if (GetAsyncKeyState(VK_Q) & 0x8000) m_keyDown |= DIR_UP;
		if (GetAsyncKeyState(VK_E) & 0x8000) m_keyDown |= DIR_DOWN;

		m_pScene->HandleInput(m_keyDown);
	}

	if (GetCapture() == m_hWnd)
	{

	}
}

void CGameFramework::UpdateObjects()
{
	m_pScene->Update(m_gameTimer.GetDeltaTime());
}

void CGameFramework::RenderObejcts()
{
	ClearFrameBuffer(RGB(100, 100, 100));

	m_pScene->Render(m_hDCFrameBuffer);

	CopyFrameBuffer();
	SetWindowText(m_hWnd, m_pszFrameRate);
}

void CGameFramework::FrameAdvance()
{
	m_gameTimer.Tick(60.0f);

	HandleInput();

	UpdateObjects();

	RenderObejcts();

	m_gameTimer.GetFrameRate(m_pszFrameRate + 12, 37);
	::SetWindowText(m_hWnd, m_pszFrameRate);
}



