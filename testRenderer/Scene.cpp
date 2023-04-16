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
	m_gameObjects.reserve(10);

	std::shared_ptr<CMesh> floorMesh = std::make_shared<CFloor>(200.0f, 0.0f, 200.0f, 20);

	m_gameObjects.emplace_back(std::make_unique<CGameObject>());
	m_gameObjects[0]->SetPickingDetection(false);
	m_gameObjects[0]->SetMesh(floorMesh);
	m_gameObjects[0]->SetColor(RGB(100, 100, 100));
	m_gameObjects[0]->SetPosition(0.0f, 0.0f, 0.0f);

	std::shared_ptr<CMesh> cubeMesh = std::make_shared<CCube>();
	m_gameObjects.emplace_back(std::make_unique<CGameObject>());
	m_gameObjects[1]->SetMesh(cubeMesh);
	m_gameObjects[1]->SetColor(RGB(0, 0, 255));
	m_gameObjects[1]->SetPosition(0.0f, 0.0f, 15.0f);
	
	m_gameObjects.emplace_back(std::make_unique<CGameObject>());
	m_gameObjects[2]->SetMesh(cubeMesh);
	m_gameObjects[2]->SetColor(RGB(0, 255, 255));
	m_gameObjects[2]->SetPosition(0.0f, -2.0f, 20.0f);

	CCamera newCamera;
	newCamera.SetPosition(XMFLOAT3A(0.0f, 0.0f, 0.0f));
	newCamera.SetCameraMatrix();
	newCamera.SetProjectMatrix(1.0f, 100.0f);
	newCamera.SetCameraProjectMatrix();

	m_pPlayer = new CTankPlayer(newCamera);
	std::shared_ptr<CMesh> tankMesh = std::make_shared<CTankMesh>();
	m_pPlayer->SetMesh(tankMesh);
	m_pPlayer->SetColor(RGB(0, 120, 0));

	m_gameObjects.emplace_back(std::make_unique<CEenemyObject>());
	m_gameObjects[3]->SetMesh(tankMesh);
	m_gameObjects[3]->SetColor(RGB(255, 0, 0));
	m_gameObjects[3]->SetPosition(0.0f, 10.0f, 20.0f);

	std::shared_ptr<CMesh> turretMesh = std::make_shared<CCube>(3.0f, 4.0f, 5.0f);
	m_pPlayer->GetChild()->SetMesh(turretMesh);
	m_pPlayer->GetChild()->SetColor(RGB(0, 150, 50));
	m_pPlayer->GetChild()->SetPosition(0.0f, 3.0f, -1.0f);

	std::shared_ptr<CMesh> gunMesh = std::make_shared<CCube>(1.0f, 1.0f, 7.0f);
	m_pPlayer->GetChild()->GetChild()->SetMesh(gunMesh);
	m_pPlayer->GetChild()->GetChild()->SetColor(RGB(100, 150, 50));
	m_pPlayer->GetChild()->GetChild()->SetPosition(0.0f, 1.0f, 6.0f);
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
		if (gameObject->CheckPicking(pickPosition, m_pPlayer->GetCamera().GetCameraMatrix(), hitDistance))
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
		//m_pPlayer->SetDirection();
		m_pPlayer->HandleInput(downKey);
	}
}

void CScene::Update(const float deltaTime)
{
	if (m_pPlayer)
	{
		m_pPlayer->Update(deltaTime);
	}

	for (std::unique_ptr<CGameObject>& gameObject : m_gameObjects)
	{
		gameObject->Update(deltaTime);
	}
}

void CScene::Collide()
{
	//충돌 처리

}

void CScene::Render(HDC hDCFrameBuffer)
{
	CGraphicsPipeline::SetCameraProejectdMatrix(m_pPlayer->GetCamera().GetCameraProjectMatrix());
	CGraphicsPipeline::SetViewport(m_pPlayer->GetCamera().GetViewport());
	
	for (std::unique_ptr<CGameObject>& gameObject : m_gameObjects)
	{
		if (m_pPlayer->GetCamera().GetWorldFrustum().Intersects(gameObject->GetOOBB())) gameObject->Render(hDCFrameBuffer);
	}
	
	m_pPlayer->Render(hDCFrameBuffer);
}
