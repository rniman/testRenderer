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
	m_gameObjects.resize(2);

	std::shared_ptr<CMesh> cubeMesh = std::make_shared<CCube>();

	m_gameObjects[0].SetMesh(cubeMesh);
	m_gameObjects[0].SetColor(RGB(0, 0, 255));
	m_gameObjects[0].SetPosition(0.0f, 0.0f, 15.0f);
	m_gameObjects[0].Rotate(45.0f, 0.0f, 0.0f);
	
	m_gameObjects[1].SetMesh(cubeMesh);
	m_gameObjects[1].SetColor(RGB(0, 255, 255));
	m_gameObjects[1].SetPosition(10.0f, 0.0f, 15.0f);
	m_gameObjects[1].Rotate(10.0f, 10.0f, 10.0f);

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
