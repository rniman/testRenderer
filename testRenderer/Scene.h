#pragma once
#include "Player.h"

class CScene
{
public:
	CScene();
	~CScene();

	CPlayer* GetPlayer() const;

	void CreateScene();
	void DestroyScene();
	
	CGameObject* GetPickedObject(const int mx, const int my);


	void HandleInput(DWORD);
	void Update(const float deltaTime = 0.01667f);
	void Collide(const float deltaTime = 0.01667f);
	void Render(HDC hDCFrameBuffer);

	void CheckPlayerByObjectCollision(const float deltaTime = 0.01667f);
	void CheckObjectByObjectCollision(const float deltaTime = 0.01667f);
	void CheckBulletByObjectCollision(const float deltaTime = 0.01667f);

private:
	std::vector<std::unique_ptr<CGameObject>> m_gameObjects;
	//std::vector<CGameObject> m_gameObjects;
	CPlayer* m_pPlayer{ nullptr };
};

