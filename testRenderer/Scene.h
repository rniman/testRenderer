#pragma once
#include "Player.h"

class CScene
{
public:
	CScene();
	~CScene();

	void CreateScene();
	void SetDownKey();

	void HandleInput(DWORD);
	void Update(const float deltaTime = 0.01667f);
	void Render(HDC hDCFrameBuffer);

private:
	std::vector<CGameObject> m_gameObjects;
	std::unique_ptr<CPlayer> m_pPlayer;

	DWORD m_downKey;
};

