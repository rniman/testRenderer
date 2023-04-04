#pragma once
#include "Scene.h"
#include "Timer.h"

class CGameFramework
{
public:
	CGameFramework();
	~CGameFramework();

	void CreateGame(HINSTANCE hInstance, HWND hWnd);
	void DestroyGame();

	void ClearFrameBuffer(DWORD color);
	void CopyFrameBuffer();

	void CreateObjects();
	void DestroyObjects();

	void HandleInput();
	void UpdateObjects();
	void RenderObejcts();

	void FrameAdvance();

private:
	bool m_Active;

	HINSTANCE m_hInstance;
	HWND m_hWnd;

	RECT m_rcClient;

	HDC m_hDCFrameBuffer;
	HBITMAP m_hBitmapFrmaeBuffer;
	HBITMAP m_hBitmapSelect;

	POINT m_ptOldCursorPos;

	_TCHAR m_pszFrameRate[50];

	DWORD m_keyDown;
	std::unique_ptr<CScene> m_pScene;

	CGameTimer m_gameTimer;
};

