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

	//카메라
	CCamera newCamera;
	newCamera.SetPosition(XMFLOAT3A(0.0f, 0.0f, 0.0f));
	newCamera.SetCameraMatrix();
	newCamera.SetProjectMatrix(1.0f, 100.0f);
	newCamera.SetCameraProjectMatrix();

	//플레이어
	m_pPlayer = new CTankPlayer(newCamera);
	std::shared_ptr<CMesh> tankMesh = std::make_shared<CTankMesh>();
	m_pPlayer->SetMesh(tankMesh);
	m_pPlayer->SetColor(RGB(0, 120, 0));

	//적 탱크 
	m_gameObjects.emplace_back(std::make_unique<CEnemyTank>());
	m_gameObjects[81]->SetMesh(tankMesh);
	m_gameObjects[81]->SetColor(RGB(255, 0, 255));
	m_gameObjects[81]->SetPosition(20.0f, 1.0f, 20.0f);
	m_gameObjects[81]->AddRotationAngle(0.0f, 0.0f, 0.0f);
	((CEnemyTank*)m_gameObjects[81].get())->SetTarget(m_pPlayer);

	m_gameObjects.emplace_back(std::make_unique<CEnemyTank>());
	m_gameObjects[82]->SetMesh(tankMesh);
	m_gameObjects[82]->SetColor(RGB(255, 0, 255));
	m_gameObjects[82]->SetPosition(90.0f, 1.0f, 90.0f);
	m_gameObjects[82]->AddRotationAngle(0.0f, 180.0f, 0.0f);
	((CEnemyTank*)m_gameObjects[81].get())->SetTarget(m_pPlayer);

	//터렛
	std::shared_ptr<CMesh> turretMesh = std::make_shared<CCube>(3.0f, 4.0f, 5.0f);
	m_gameObjects[81]->GetChild()->SetMesh(turretMesh);
	m_gameObjects[81]->GetChild()->SetColor(RGB(255, 0, 255));
	m_gameObjects[81]->GetChild()->SetPosition(0.0f, 3.0f, -1.0f);

	m_gameObjects[82]->GetChild()->SetMesh(turretMesh);
	m_gameObjects[82]->GetChild()->SetColor(RGB(255, 0, 255));
	m_gameObjects[82]->GetChild()->SetPosition(0.0f, 3.0f, -1.0f);

	m_pPlayer->GetChild()->SetMesh(turretMesh);
	m_pPlayer->GetChild()->SetColor(RGB(0, 150, 50));
	m_pPlayer->GetChild()->SetPosition(0.0f, 3.0f, -1.0f);

	//대포
	std::shared_ptr<CMesh> gunMesh = std::make_shared<CStick>(1.0f, 1.0f, 7.0f);
	m_gameObjects[81]->GetChild()->GetChild()->SetMesh(gunMesh);
	m_gameObjects[81]->GetChild()->GetChild()->SetColor(RGB(255, 0, 255));
	m_gameObjects[81]->GetChild()->GetChild()->SetPosition(0.0f, 1.0f, 2.5f);

	m_gameObjects[82]->GetChild()->GetChild()->SetMesh(gunMesh);
	m_gameObjects[82]->GetChild()->GetChild()->SetColor(RGB(255, 0, 255));
	m_gameObjects[82]->GetChild()->GetChild()->SetPosition(0.0f, 1.0f, 2.5f);

	m_pPlayer->GetChild()->GetChild()->SetMesh(gunMesh);
	m_pPlayer->GetChild()->GetChild()->SetColor(RGB(100, 150, 50));
	m_pPlayer->GetChild()->GetChild()->SetPosition(0.0f, 1.0f, 2.5f);
}

void CScene::DestroyScene()
{
	delete m_pPlayer;
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
	m_pPlayer->SetInactiveMoveForce();
	if (downKey)
	{
		m_pPlayer->HandleInput(downKey);
	}
}

void CScene::Update(const float deltaTime)
{
	m_pPlayer->Update(deltaTime);

	for (const std::unique_ptr<CGameObject>& gameObject : m_gameObjects)
	{
		if (!gameObject->GetActive())
		{
			continue;
		}

		gameObject->Update(deltaTime);
	}
}

void CScene::Collide(const float deltaTime)
{
	//충돌 처리
	CheckPlayerByObjectCollision();

	//CheckObjectByObjectCollision();

	CheckBulletByObjectCollision();
}

void CScene::Render(HDC hDCFrameBuffer)
{
	CGraphicsPipeline::SetCameraProejectdMatrix(m_pPlayer->GetCamera().GetCameraProjectMatrix());
	CGraphicsPipeline::SetViewport(m_pPlayer->GetCamera().GetViewport());
	
	m_pPlayer->Render(hDCFrameBuffer);

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

		//임시로 설정
		if (!gameObject->GetCollision())
		{
			continue;
		}

		if (playerOOBB.Intersects(gameObject->GetOOBB()))
		{
			gameObject->SetCollidedObject(m_pPlayer);
			m_pPlayer->SetCollidedObject(gameObject.get());
		}
	}

	if (m_pPlayer->GetCollidedObject())
	{
		m_pPlayer->Collide(deltaTime);
	}
}

void CScene::CheckObjectByObjectCollision(const float deltaTime)
{

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
				continue;
			}

			//서로 충돌 처리
			bullet.DeleteBullet();
			gameObject->SetCollidedObject(&bullet);
			gameObject->Collide(deltaTime);
		}
	}

	for (CBulletObject& bullet : static_cast<CEnemyTank*>(m_gameObjects[81].get())->GetBullets())
	{
		if (!bullet.GetActive())
		{
			continue;
		}

		for (int i = 0; i < 81; ++i)
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

	for (CBulletObject& bullet : static_cast<CEnemyTank*>(m_gameObjects[82].get())->GetBullets())
	{
		if (!bullet.GetActive())
		{
			continue;
		}

		for (int i = 0; i < 81; ++i)
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

