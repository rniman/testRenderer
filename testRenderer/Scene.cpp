#include "stdafx.h"
#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

const std::unique_ptr<CPlayer>& CScene::GetPlayer() const
{
	return m_pPlayer;
}

void CScene::CreateScene()
{
	m_gameObjects.reserve(10);

	std::shared_ptr<CMesh> floorMesh = std::make_shared<CFloor>(200.0f, 0.0f, 200.0f, 20);

	m_gameObjects.emplace_back();
	m_gameObjects[0].SetPickingDetection(false);
	m_gameObjects[0].SetMesh(floorMesh);
	m_gameObjects[0].SetColor(RGB(100, 100, 100));
	m_gameObjects[0].SetPosition(0.0f, 0.0f, 0.0f);

	std::shared_ptr<CMesh> cubeMesh = std::make_shared<CCube>();
	m_gameObjects.emplace_back();
	m_gameObjects[1].SetMesh(cubeMesh);
	m_gameObjects[1].SetColor(RGB(0, 0, 255));
	m_gameObjects[1].SetPosition(0.0f, 0.0f, 15.0f);
	
	m_gameObjects.emplace_back();
	m_gameObjects[2].SetMesh(cubeMesh);
	m_gameObjects[2].SetColor(RGB(0, 255, 255));
	m_gameObjects[2].SetPosition(10.0f, 2.0f, 15.0f);

	CCamera newCamera;
	newCamera.SetPosition(XMFLOAT3A(0.0f, 0.0f, 0.0f));
	newCamera.SetCameraMatrix();
	newCamera.SetProjectMatrix(1.0f, 100.0f);
	newCamera.SetCameraProjectMatrix();

	m_pPlayer = std::make_unique<CTankPlayer>(newCamera);
	std::shared_ptr<CMesh> tankMesh = std::make_shared<CTankMesh>();
	m_pPlayer->SetMesh(tankMesh);
	m_pPlayer->SetColor(RGB(0, 120, 0));
}

CGameObject* CScene::GetPickedObject(const int mx, const int my)
{
	XMFLOAT3A pickPosition;
	pickPosition.x = ((2.0f * mx) /  (float)FRAMEBUFFER_WIDTH - 1) / m_pPlayer->GetCamera().GetPerspectiveProjectMatrix()._11;
	pickPosition.y = -(((2.0f * my) / (float)FRAMEBUFFER_HEIGHT - 1) / m_pPlayer->GetCamera().GetPerspectiveProjectMatrix()._22);
	pickPosition.z = 1.0f;

	//XMFLOAT3A c = { 0.0f, 0.0f, 0.0f };
	//XMFLOAT4X4A b = m_pPlayer->GetCamera().GetCameraMatrix();
	//XMMATRIX a = XMLoadFloat4x4A(&b);
	//XMFLOAT3A pick;
	//XMFLOAT3A ca;
	//XMStoreFloat3A(&pick, XMVector3TransformCoord(XMLoadFloat3A(&pickPosition), XMMatrixInverse(nullptr, a)));
	//XMStoreFloat3A(&ca, XMVector3TransformCoord(XMLoadFloat3A(&c), XMMatrixInverse(nullptr, a)));

	int pickObjectIndex = -1;
	int objectIndex = 0;
	float nearestHitDistance = FLT_MAX;
	for (CGameObject& gameObject : m_gameObjects)
	{
		if (!gameObject.GetPickingDetection())
		{
			objectIndex++;
			continue;
		}

		float hitDistance = FLT_MAX;
		if (gameObject.CheckPicking(pickPosition, m_pPlayer->GetCamera().GetCameraMatrix(), hitDistance))
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
		return &m_gameObjects[pickObjectIndex];
	}
	else
	{
		return nullptr;
	}
}

void CScene::HandleInput(DWORD downKey)
{
	m_pPlayer->SetDirection();
	if (downKey)
	{
		m_pPlayer->HandleInput(downKey);
	}
}

void CScene::Update(const float deltaTime)
{
	for (CGameObject& gameObject : m_gameObjects)
	{
		gameObject.Update(deltaTime);
	}

	if (m_pPlayer)
	{
		m_pPlayer->Update(deltaTime);
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
	
	for (CGameObject& gameObject : m_gameObjects)
	{
		gameObject.Render(hDCFrameBuffer);
	}
	
	m_pPlayer->Render(hDCFrameBuffer);
}
