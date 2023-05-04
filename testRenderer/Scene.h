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

	void CreateEnemy(std::shared_ptr<CMesh>& tankMesh, std::shared_ptr<CMesh>& turretMesh, std::shared_ptr<CMesh>& gunMesh);

	CGameObject* GetPickedObject(const int mx, const int my);

	void HandleInput(DWORD);
	void Update(const float deltaTime = 0.01667f);
	void Collide(const float deltaTime = 0.01667f);
	void Render(HDC hDCFrameBuffer);

	void CheckPlayerByObjectCollision(const float deltaTime = 0.01667f);
	void CheckEnemyByObjectCollision(const float deltaTime = 0.01667f);
	void CheckBulletByObjectCollision(const float deltaTime = 0.01667f);

	void CheckBulletByPlayer(std::unique_ptr<CGameObject>& gameObject, float deltaTime);

private:
	std::vector<std::unique_ptr<CGameObject>> m_gameObjects;
	CPlayer* m_pPlayer{ nullptr };
	size_t m_enviromentsNum{};
	size_t m_enemyNum{};
	size_t m_inActiveEnemyNum{};
};

