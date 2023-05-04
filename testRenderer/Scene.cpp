#include "stdafx.h"
#include "Scene.h"
#include "EenemyObject.h"

CScene::CScene()
	: m_gameObjects{}
	, m_pPlayer{ nullptr }
{
}

CScene::~CScene()
{
	if (m_pPlayer)
	{
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}
}

CPlayer* CScene::GetPlayer() const
{
	return m_pPlayer;
}

void CScene::CreateScene()
{
	m_gameObjects.reserve(100);

	std::shared_ptr<CMesh> floorMesh = std::make_shared<CFloor>(200.0f, 0.0f, 200.0f, 20);
	m_gameObjects.emplace_back(std::make_unique<CGameObject>());
	m_gameObjects[0]->SetPickingDetection(false);
	m_gameObjects[0]->SetMesh(floorMesh);
	m_gameObjects[0]->SetColor(RGB(100, 100, 100));
	m_gameObjects[0]->SetPosition(0.0f, 0.0f, 0.0f);
	m_gameObjects[0]->SetStatic(true);
	m_gameObjects[0]->SetOOBB();
	m_gameObjects[0]->SetCollision(false);

	std::shared_ptr<CMesh> obstacleMesh = std::make_shared<CCube>(10.0f, 10.0f, 10.0f);
	for (int i = 1; i < 21; ++i)
	{
		m_gameObjects.emplace_back(std::make_unique<CGameObject>());
		m_gameObjects[i]->SetMesh(obstacleMesh);
		m_gameObjects[i]->SetColor(RGB(50, 50, 50));
		m_gameObjects[i]->SetPosition(-100.0f + 10.0f * (i - 1) + 5.0f, 5.0f, 105.0f);
		m_gameObjects[i]->SetStatic(true);
		m_gameObjects[i]->SetOOBB();
	}

	for (int i = 21; i < 41; ++i)
	{
		m_gameObjects.emplace_back(std::make_unique<CGameObject>());
		m_gameObjects[i]->SetMesh(obstacleMesh);
		m_gameObjects[i]->SetColor(RGB(50, 50, 50));
		m_gameObjects[i]->SetPosition(-100.0f + 10.0f * (i - 21) + 5.0f, 5.0f, -105.0f);
		m_gameObjects[i]->SetStatic(true);
		m_gameObjects[i]->SetOOBB();
	}

	for (int i = 41; i < 61; ++i)
	{
		m_gameObjects.emplace_back(std::make_unique<CGameObject>());
		m_gameObjects[i]->SetMesh(obstacleMesh);
		m_gameObjects[i]->SetColor(RGB(50, 50, 50));
		m_gameObjects[i]->SetPosition(105.0f, 5.0f, -100.0f + 10.0f * (i - 41) + 5.0f);
		m_gameObjects[i]->SetStatic(true);
		m_gameObjects[i]->SetOOBB();
	}

	for (int i = 61; i < 81; ++i)
	{
		m_gameObjects.emplace_back(std::make_unique<CGameObject>());
		m_gameObjects[i]->SetMesh(obstacleMesh);
		m_gameObjects[i]->SetColor(RGB(50, 50, 50));
		m_gameObjects[i]->SetPosition(-105.0f, 5.0f, -100.0f + 10.0f * (i - 61) + 5.0f);
		m_gameObjects[i]->SetStatic(true);
		m_gameObjects[i]->SetOOBB();
	}

	m_gameObjects.emplace_back(std::make_unique<CGameObject>());
	m_gameObjects[81]->SetMesh(obstacleMesh);
	m_gameObjects[81]->SetColor(RGB(50, 150, 150));
	m_gameObjects[81]->SetPosition(-45.0f, 5.0f, 45.0f);
	m_gameObjects[81]->SetStatic(true);
	m_gameObjects[81]->SetOOBB();

	m_gameObjects.emplace_back(std::make_unique<CGameObject>());
	m_gameObjects[82]->SetMesh(obstacleMesh);
	m_gameObjects[82]->SetColor(RGB(50, 150, 150));
	m_gameObjects[82]->SetPosition(-45.0f, 5.0f, -45.0f);
	m_gameObjects[82]->SetStatic(true);
	m_gameObjects[82]->SetOOBB();

	m_gameObjects.emplace_back(std::make_unique<CGameObject>());
	m_gameObjects[83]->SetMesh(obstacleMesh);
	m_gameObjects[83]->SetColor(RGB(50, 150, 150));
	m_gameObjects[83]->SetPosition(45.0f, 5.0f, -45.0f);
	m_gameObjects[83]->SetStatic(true);
	m_gameObjects[83]->SetOOBB();

	m_gameObjects.emplace_back(std::make_unique<CGameObject>());
	m_gameObjects[84]->SetMesh(obstacleMesh);
	m_gameObjects[84]->SetColor(RGB(50, 150, 150));
	m_gameObjects[84]->SetPosition(45.0f, 5.0f, 45.0f);
	m_gameObjects[84]->SetStatic(true);
	m_gameObjects[84]->SetOOBB();

	m_enviromentsNum = m_gameObjects.size();

	//카메라
	CCamera newCamera;
	newCamera.SetPosition(XMFLOAT3A(0.0f, 0.0f, 0.0f));
	newCamera.SetCameraMatrix();
	newCamera.SetProjectMatrix(1.0f, 150.0f);
	newCamera.SetCameraProjectMatrix();

	//플레이어
	m_pPlayer = new CTankPlayer(newCamera);
	std::shared_ptr<CMesh> tankMesh = std::make_shared<CTankMesh>();
	m_pPlayer->SetMesh(tankMesh);
	m_pPlayer->SetColor(RGB(0, 120, 0));
	m_pPlayer->SetPosition(0.0f, 1.0f, -45.0f);

	//터렛
	std::shared_ptr<CMesh> turretMesh = std::make_shared<CCube>(3.0f, 4.0f, 5.0f);

	m_pPlayer->GetChild()->SetMesh(turretMesh);
	m_pPlayer->GetChild()->SetColor(RGB(0, 150, 50));
	m_pPlayer->GetChild()->SetPosition(0.0f, 3.0f, -1.0f);

	//대포
	std::shared_ptr<CMesh> gunMesh = std::make_shared<CStick>(1.0f, 1.0f, 7.0f);

	m_pPlayer->GetChild()->GetChild()->SetMesh(gunMesh);
	m_pPlayer->GetChild()->GetChild()->SetColor(RGB(100, 150, 50));
	m_pPlayer->GetChild()->GetChild()->SetPosition(0.0f, 1.0f, 2.5f);

	CreateEnemy(tankMesh, turretMesh, gunMesh);
}

void CScene::DestroyScene()
{
	delete m_pPlayer;
}

void CScene::CreateEnemy(std::shared_ptr<CMesh>& tankMesh, std::shared_ptr<CMesh>& turretMesh, std::shared_ptr<CMesh>& gunMesh)
{
	m_gameObjects.emplace_back(std::make_unique<CEnemyTank>());
	m_gameObjects[85]->SetMesh(tankMesh);
	m_gameObjects[85]->SetColor(RGB(255, 0, 255));
	m_gameObjects[85]->SetPosition(90.0f, 1.0f, 0.0f);
	m_gameObjects[85]->AddRotationAngle(0.0f, 180.0f, 0.0f);
	((CEnemyTank*)m_gameObjects[85].get())->SetTarget(m_pPlayer);

	m_gameObjects[85]->GetChild()->SetMesh(turretMesh);
	m_gameObjects[85]->GetChild()->SetColor(RGB(255, 0, 255));
	m_gameObjects[85]->GetChild()->SetPosition(0.0f, 3.0f, -1.0f);

	m_gameObjects[85]->GetChild()->GetChild()->SetMesh(gunMesh);
	m_gameObjects[85]->GetChild()->GetChild()->SetColor(RGB(255, 0, 255));
	m_gameObjects[85]->GetChild()->GetChild()->SetPosition(0.0f, 1.0f, 2.5f);


	m_gameObjects.emplace_back(std::make_unique<CEnemyTank>());
	m_gameObjects[86]->SetMesh(tankMesh);
	m_gameObjects[86]->SetColor(RGB(255, 0, 255));
	m_gameObjects[86]->SetPosition(0.0f, 1.0f, 90.0f);
	m_gameObjects[86]->AddRotationAngle(0.0f, 180.0f, 0.0f);
	((CEnemyTank*)m_gameObjects[86].get())->SetTarget(m_pPlayer);

	m_gameObjects[86]->GetChild()->GetChild()->SetMesh(gunMesh);
	m_gameObjects[86]->GetChild()->GetChild()->SetColor(RGB(255, 0, 255));
	m_gameObjects[86]->GetChild()->GetChild()->SetPosition(0.0f, 1.0f, 2.5f);

	m_gameObjects[86]->GetChild()->SetMesh(turretMesh);
	m_gameObjects[86]->GetChild()->SetColor(RGB(255, 0, 255));
	m_gameObjects[86]->GetChild()->SetPosition(0.0f, 3.0f, -1.0f);

	m_gameObjects.emplace_back(std::make_unique<CEnemyTank>());
	m_gameObjects[87]->SetMesh(tankMesh);
	m_gameObjects[87]->SetColor(RGB(255, 0, 255));
	m_gameObjects[87]->SetPosition(-90.0f, 1.0f, 0.0f);
	m_gameObjects[87]->AddRotationAngle(0.0f, 180.0f, 0.0f);
	((CEnemyTank*)m_gameObjects[87].get())->SetTarget(m_pPlayer);

	m_gameObjects[87]->GetChild()->SetMesh(turretMesh);
	m_gameObjects[87]->GetChild()->SetColor(RGB(255, 0, 255));
	m_gameObjects[87]->GetChild()->SetPosition(0.0f, 3.0f, -1.0f);

	m_gameObjects[87]->GetChild()->GetChild()->SetMesh(gunMesh);
	m_gameObjects[87]->GetChild()->GetChild()->SetColor(RGB(255, 0, 255));
	m_gameObjects[87]->GetChild()->GetChild()->SetPosition(0.0f, 1.0f, 2.5f);

	m_gameObjects.emplace_back(std::make_unique<CEnemyTank>());
	m_gameObjects[88]->SetMesh(tankMesh);
	m_gameObjects[88]->SetColor(RGB(255, 0, 255));
	m_gameObjects[88]->SetPosition(-45.0f, 11.0f, 45.0f);
	m_gameObjects[88]->AddRotationAngle(0.0f, 135.0f, 0.0f);
	((CEnemyTank*)m_gameObjects[88].get())->SetTarget(m_pPlayer);
	((CEnemyTank*)m_gameObjects[88].get())->SetMove(false);

	m_gameObjects[88]->GetChild()->SetMesh(turretMesh);
	m_gameObjects[88]->GetChild()->SetColor(RGB(255, 0, 255));
	m_gameObjects[88]->GetChild()->SetPosition(0.0f, 3.0f, -1.0f);

	m_gameObjects[88]->GetChild()->GetChild()->SetMesh(gunMesh);
	m_gameObjects[88]->GetChild()->GetChild()->SetColor(RGB(255, 0, 255));
	m_gameObjects[88]->GetChild()->GetChild()->SetPosition(0.0f, 1.0f, 2.5f);

	m_gameObjects.emplace_back(std::make_unique<CEnemyTank>());
	m_gameObjects[89]->SetMesh(tankMesh);
	m_gameObjects[89]->SetColor(RGB(255, 0, 255));
	m_gameObjects[89]->SetPosition(45.0f, 11.0f, 45.0f);
	m_gameObjects[89]->AddRotationAngle(0.0f, -135.0f, 0.0f);
	((CEnemyTank*)m_gameObjects[89].get())->SetTarget(m_pPlayer);
	((CEnemyTank*)m_gameObjects[89].get())->SetMove(false);

	m_gameObjects[89]->GetChild()->SetMesh(turretMesh);
	m_gameObjects[89]->GetChild()->SetColor(RGB(255, 0, 255));
	m_gameObjects[89]->GetChild()->SetPosition(0.0f, 3.0f, -1.0f);

	m_gameObjects[89]->GetChild()->GetChild()->SetMesh(gunMesh);
	m_gameObjects[89]->GetChild()->GetChild()->SetColor(RGB(255, 0, 255));
	m_gameObjects[89]->GetChild()->GetChild()->SetPosition(0.0f, 1.0f, 2.5f);

	m_gameObjects.emplace_back(std::make_unique<CEnemyTank>());
	m_gameObjects[90]->SetMesh(tankMesh);
	m_gameObjects[90]->SetColor(RGB(255, 0, 255));
	m_gameObjects[90]->SetPosition(-45.0f, 11.0f, -45.0f);
	m_gameObjects[90]->AddRotationAngle(0.0f, 45.0f, 0.0f);
	((CEnemyTank*)m_gameObjects[90].get())->SetTarget(m_pPlayer);
	((CEnemyTank*)m_gameObjects[90].get())->SetMove(false);

	m_gameObjects[90]->GetChild()->SetMesh(turretMesh);
	m_gameObjects[90]->GetChild()->SetColor(RGB(255, 0, 255));
	m_gameObjects[90]->GetChild()->SetPosition(0.0f, 3.0f, -1.0f);

	m_gameObjects[90]->GetChild()->GetChild()->SetMesh(gunMesh);
	m_gameObjects[90]->GetChild()->GetChild()->SetColor(RGB(255, 0, 255));
	m_gameObjects[90]->GetChild()->GetChild()->SetPosition(0.0f, 1.0f, 2.5f);

	m_gameObjects.emplace_back(std::make_unique<CEnemyTank>());
	m_gameObjects[91]->SetMesh(tankMesh);
	m_gameObjects[91]->SetColor(RGB(255, 0, 255));
	m_gameObjects[91]->SetPosition(45.0f, 11.0f, -45.0f);
	m_gameObjects[91]->AddRotationAngle(0.0f, -45.0f, 0.0f);
	((CEnemyTank*)m_gameObjects[91].get())->SetTarget(m_pPlayer);
	((CEnemyTank*)m_gameObjects[91].get())->SetMove(false);

	m_gameObjects[91]->GetChild()->SetMesh(turretMesh);
	m_gameObjects[91]->GetChild()->SetColor(RGB(255, 0, 255));
	m_gameObjects[91]->GetChild()->SetPosition(0.0f, 3.0f, -1.0f);

	m_gameObjects[91]->GetChild()->GetChild()->SetMesh(gunMesh);
	m_gameObjects[91]->GetChild()->GetChild()->SetColor(RGB(255, 0, 255));
	m_gameObjects[91]->GetChild()->GetChild()->SetPosition(0.0f, 1.0f, 2.5f);

	m_enemyNum = 7;
}

CGameObject* CScene::GetPickedObject(const int mx, const int my)
{
	XMFLOAT3A pickPosition;
	pickPosition.x = ((2.0f * mx) /  (float)FRAMEBUFFER_WIDTH - 1) / m_pPlayer->GetCamera().GetPerspectiveProjectMatrix()._11;
	pickPosition.y = -(((2.0f * my) / (float)FRAMEBUFFER_HEIGHT - 1) / m_pPlayer->GetCamera().GetPerspectiveProjectMatrix()._22);
	pickPosition.z = 1.0f;

	int pickObjectIndex = -1;
	int objectIndex = 0;
	float nearestHitDistance = FLT_MAX;
	for (std::unique_ptr<CGameObject>& gameObject : m_gameObjects)
	{
		if (!gameObject->GetPickingDetection())
		{
			objectIndex++;
			continue;
		}

		float hitDistance = FLT_MAX;
		if (CGameObject::CheckPicking(gameObject.get(), pickPosition, m_pPlayer->GetCamera().GetCameraMatrix(), hitDistance))
		{
			if (hitDistance < nearestHitDistance)
			{
				nearestHitDistance = hitDistance;
				pickObjectIndex = objectIndex;
			}
		}
		objectIndex++;
	}
	
	if (pickObjectIndex >= 0)
	{
		return m_gameObjects[pickObjectIndex].get();
	}
	else
	{
		return nullptr;
	}
}

void CScene::HandleInput(DWORD downKey)
{
	if (!m_pPlayer->GetActive())
	{
		return;
	}

	m_pPlayer->SetInactiveMoveForce();
	if (downKey)
	{
		m_pPlayer->HandleInput(downKey);
	}
}

void CScene::Update(const float deltaTime)
{
	if (m_pPlayer->GetActive())
	{
		m_pPlayer->Update(deltaTime);
	}

	m_inActiveEnemyNum = 0;
	for (const std::unique_ptr<CGameObject>& gameObject : m_gameObjects)
	{
		if (!gameObject->GetActive())
		{
			m_inActiveEnemyNum++;
			continue;
		}

		gameObject->Update(deltaTime);
	}
}

void CScene::Collide(const float deltaTime)
{
	//충돌 처리
	CheckPlayerByObjectCollision();

	CheckEnemyByObjectCollision();

	CheckBulletByObjectCollision();
}

void CScene::Render(HDC hDCFrameBuffer)
{
	CGraphicsPipeline::SetCameraProejectdMatrix(m_pPlayer->GetCamera().GetCameraProjectMatrix());
	CGraphicsPipeline::SetViewport(m_pPlayer->GetCamera().GetViewport());
	
	if (m_inActiveEnemyNum == m_enemyNum)
	{
		RECT rect{ 0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT - 200 };
		TCHAR gameOver[] = _T("GAME CLEAR");
		TCHAR exit[] = _T("ESC key - EXIT");

		HFONT newFont = CreateFont(100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		HFONT oldFont = (HFONT)SelectObject(hDCFrameBuffer, newFont);
		SetTextColor(hDCFrameBuffer, RGB(255, 255, 255));
		DrawText(hDCFrameBuffer, gameOver, _tcslen(gameOver), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		rect.bottom = FRAMEBUFFER_HEIGHT;
		DrawText(hDCFrameBuffer, exit, _tcslen(exit), &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

		SelectObject(hDCFrameBuffer, oldFont);
		DeleteObject(newFont);
	}
	else if (m_pPlayer->GetActive())
	{
		m_pPlayer->Render(hDCFrameBuffer);
		UINT player_hp = ((CTankPlayer*)m_pPlayer)->GetHp();
		RECT rect{ 0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT };
		TCHAR hp[4];
		_ui64tot_s(player_hp, hp, 4, 10);
		HFONT newFont = CreateFont(80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		HFONT oldFont = (HFONT)SelectObject(hDCFrameBuffer, newFont);
		SetTextColor(hDCFrameBuffer, RGB(255, 255, 255));
		DrawText(hDCFrameBuffer, hp, _tcslen(hp), &rect, DT_LEFT | DT_SINGLELINE | DT_BOTTOM);
		SelectObject(hDCFrameBuffer, oldFont);
		DeleteObject(newFont);
	}
	else
	{
		RECT rect{ 0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT - 200 };
		TCHAR gameOver[] = _T("GAME OVER");
		TCHAR exit[] = _T("ESC key - EXIT");

		HFONT newFont = CreateFont(100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		HFONT oldFont = (HFONT)SelectObject(hDCFrameBuffer, newFont);
		SetTextColor(hDCFrameBuffer, RGB(255, 255, 255));
		DrawText(hDCFrameBuffer, gameOver, _tcslen(gameOver), &rect, DT_CENTER |DT_SINGLELINE | DT_VCENTER);
		rect.bottom = FRAMEBUFFER_HEIGHT;
		DrawText(hDCFrameBuffer, exit, _tcslen(exit), &rect, DT_CENTER |DT_SINGLELINE | DT_VCENTER);

		SelectObject(hDCFrameBuffer, oldFont);
		DeleteObject(newFont);
	}

	for (const std::unique_ptr<CGameObject>& gameObject : m_gameObjects)
	{
		if (!gameObject->GetActive())
		{
			continue;
		}

		if (m_pPlayer->GetCamera().GetWorldFrustum().Intersects(gameObject->GetOOBB()))
		{
			gameObject->Render(hDCFrameBuffer);
		}
		else if (dynamic_cast<CEnemyTank*>(gameObject.get()))
		{
			for (CBulletObject& bullet : static_cast<CEnemyTank*>(gameObject.get())->GetBullets())
			{
				if (!bullet.GetActive())
				{
					continue;
				}

				bullet.Render(hDCFrameBuffer);
			}
		}

	}
}

void CScene::CheckPlayerByObjectCollision(const float deltaTime)
{
	if (!m_pPlayer->GetActive())
	{
		return;
	}

	BoundingOrientedBox playerOOBB = m_pPlayer->GetOOBB();
	m_pPlayer->SetCollidedObject(nullptr);

	for (const std::unique_ptr<CGameObject>& gameObject : m_gameObjects)
	{
		if (!gameObject->GetActive())
		{
			continue;
		}

		gameObject->SetCollidedObject(nullptr);
		if (m_pPlayer->GetCollidedObject())
		{
			continue;
		}

		if (!gameObject->GetCollision())
		{
			continue;
		}

		if (playerOOBB.Intersects(gameObject->GetOOBB()))
		{
			gameObject->SetCollidedObject(m_pPlayer);
			m_pPlayer->SetCollidedObject(gameObject.get());

			gameObject->Collide(deltaTime);
		}
	}

	if (m_pPlayer->GetCollidedObject())
	{
		m_pPlayer->Collide(deltaTime);
	}
}

void CScene::CheckEnemyByObjectCollision(const float deltaTime)
{
	m_gameObjects[85]->SetCollidedObject(nullptr);
	m_gameObjects[86]->SetCollidedObject(nullptr);
	m_gameObjects[87]->SetCollidedObject(nullptr);

	if (m_gameObjects[85]->GetActive() && m_gameObjects[86]->GetActive())
	{
		if (m_gameObjects[85]->GetOOBB().Intersects(m_gameObjects[86]->GetOOBB()))
		{
			m_gameObjects[85]->SetCollidedObject(m_gameObjects[86].get());
			m_gameObjects[86]->SetCollidedObject(m_gameObjects[85].get());
		}
	}

	if (m_gameObjects[85]->GetActive() && m_gameObjects[87]->GetActive())
	{
		if (m_gameObjects[85]->GetOOBB().Intersects(m_gameObjects[87]->GetOOBB()))
		{
			m_gameObjects[85]->SetCollidedObject(m_gameObjects[87].get());
			m_gameObjects[87]->SetCollidedObject(m_gameObjects[85].get());
		}
	}

	if (m_gameObjects[86]->GetActive() && m_gameObjects[87]->GetActive())
	{
		if (m_gameObjects[86]->GetOOBB().Intersects(m_gameObjects[87]->GetOOBB()))
		{
			m_gameObjects[86]->SetCollidedObject(m_gameObjects[87].get());
			m_gameObjects[87]->SetCollidedObject(m_gameObjects[86].get());
		}
	}

	for (int i = 85; i < 88; ++i)
	{
		if (!m_gameObjects[i]->GetActive())
		{
			continue;
		}

		for (int j = 0; j < m_enviromentsNum; ++j)
		{
			if (!m_gameObjects[j]->GetCollision())
			{
				continue;
			}

			if (!m_gameObjects[i]->GetOOBB().Intersects(m_gameObjects[j]->GetOOBB()))
			{
				continue;
			}

			m_gameObjects[i]->SetCollidedObject(m_gameObjects[j].get());
		}

		if (m_gameObjects[i]->GetCollidedObject())
		{
			m_gameObjects[i]->Collide(deltaTime);
		}
	}
}

void CScene::CheckBulletByObjectCollision(const float deltaTime)
{
	for (CBulletObject& bullet : static_cast<CTankPlayer*>(m_pPlayer)->GetBullets())
	{
		if (!bullet.GetActive())
		{
			continue;
		}

		for (const std::unique_ptr<CGameObject>& gameObject : m_gameObjects)
		{
			gameObject->SetCollidedObject(nullptr);
			if (!gameObject->GetActive())
			{
				continue;
			}
			
			CEnemyTank* enemy = dynamic_cast<CEnemyTank*>(gameObject.get());
			if (!bullet.GetOOBB().Intersects(gameObject->GetOOBB()))
			{
				if (!enemy)
				{
					continue;
				}
				
				if (!bullet.GetOOBB().Intersects(enemy->GetTurret()->GetOOBB()) && !bullet.GetOOBB().Intersects(enemy->GetGun()->GetOOBB()))
				{
					continue;
				}
			}
			
			if (!gameObject->GetPickingDetection() || !gameObject->GetCollision())
			{
				//총만 비활성화
				bullet.DeleteBullet();
				break;
			}

			//서로 충돌 처리
			bullet.DeleteBullet();
			gameObject->SetCollidedObject(&bullet);
			gameObject->Collide(deltaTime);
		}
	}

	CheckBulletByPlayer(m_gameObjects[85], deltaTime);
	CheckBulletByPlayer(m_gameObjects[86], deltaTime);
	CheckBulletByPlayer(m_gameObjects[87], deltaTime);
	CheckBulletByPlayer(m_gameObjects[88], deltaTime);
	CheckBulletByPlayer(m_gameObjects[89], deltaTime);
	CheckBulletByPlayer(m_gameObjects[90], deltaTime);
	CheckBulletByPlayer(m_gameObjects[91], deltaTime);
}

void CScene::CheckBulletByPlayer(std::unique_ptr<CGameObject>& gameObject, float deltaTime)
{
	for (CBulletObject& bullet : static_cast<CEnemyTank*>(gameObject.get())->GetBullets())
	{
		if (!bullet.GetActive())
		{
			continue;
		}

		CTankPlayer* player = dynamic_cast<CTankPlayer*>(m_pPlayer);
		if (player->GetActive() && (bullet.GetOOBB().Intersects(player->GetOOBB()) || bullet.GetOOBB().Intersects(player->GetTurret()->GetOOBB()) || bullet.GetOOBB().Intersects(player->GetGun()->GetOOBB())))
		{
			bullet.DeleteBullet();
			player->SetCollidedObject(&bullet);
			player->Collide(deltaTime);
			continue;
		}

		for (int i = 0; i < m_enviromentsNum; ++i)
		{
			if (!m_gameObjects[i]->GetActive())
			{
				continue;
			}

			if (!bullet.GetOOBB().Intersects(m_gameObjects[i]->GetOOBB()))
			{
				continue;
			}

			bullet.DeleteBullet();
			break;
		}
	}
}

 