#pragma once
#include "Player.h"

class CScene
{
public:
	CScene();
	~CScene();

	const std::unique_ptr<CPlayer>& GetPlayer() const;

	void CreateScene();
	
	CGameObject* GetPickedObject(const int mx, const int my);

	void HandleInput(DWORD);
	void Update(const float deltaTime = 0.01667f);
	void Collide();
	void Render(HDC hDCFrameBuffer);

private:
	std::vector<CGameObject> m_gameObjects;
	std::unique_ptr<CPlayer> m_pPlayer;
};

