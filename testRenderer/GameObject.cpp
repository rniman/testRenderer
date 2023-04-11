#include "stdafx.h"
#include "GameObject.h"

CGameObject::CGameObject()
	: m_active{ true }
	, m_mesh{ nullptr }
	, m_color{ RGB(255, 0, 0) }
	, m_position{0.0f, 0.0f, 0.0f}
	, m_totalRotation{ 0.0f, 0.0f, 0.0f }
	, m_moveSpeed{ 0.0f }
	, m_rotationSpeed{ 0.0f }
	, m_pickingDetection{ true }
{
	XMStoreFloat4x4A(&m_worldMatrix, XMMatrixIdentity());
}

CGameObject::~CGameObject()
{
}

bool CGameObject::GetActive() const
{
	return m_active;
}

bool CGameObject::GetPickingDetection() const
{
	return m_pickingDetection;
}

void CGameObject::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;

	m_worldMatrix._41 = x;
	m_worldMatrix._42 = y;
	m_worldMatrix._43 = z;
}

void CGameObject::SetPosition(const XMFLOAT3A& position)
{
	m_position = position;

	m_worldMatrix._41 = position.x;
	m_worldMatrix._42 = position.y;
	m_worldMatrix._43 = position.z;
}

void CGameObject::SetMesh(const std::shared_ptr<CMesh>& mesh)
{
	m_mesh = mesh;
}

void CGameObject::SetColor(const DWORD color)
{
	m_color = color;
}

void CGameObject::SetPickingDetection(const bool detection)
{
	m_pickingDetection = detection;
}

void CGameObject::SetOOBB()
{
	m_mesh->GetOOBB().Transform(m_OOBB, XMLoadFloat4x4A(&m_worldMatrix));
	XMStoreFloat4(&m_OOBB.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_OOBB.Orientation)));
}

void CGameObject::AddRotationAngle(const float pitch, const float yaw, const float roll)
{
	AddRotationAngle(XMFLOAT3A(pitch, yaw, roll));
}

void CGameObject::AddRotationAngle(const XMFLOAT3A& rotate)
{
	XMStoreFloat3A(&m_totalRotation, XMLoadFloat3A(&m_totalRotation) + XMLoadFloat3(&rotate));
}

bool CGameObject::CheckPicking(const XMFLOAT3A& pickPosition, const XMFLOAT4X4A& cameraMatrix, float& distance)
{
	XMMATRIX modelMatrix = XMMatrixInverse(nullptr, XMLoadFloat4x4A(&m_worldMatrix) * XMLoadFloat4x4A(&cameraMatrix));
	
	XMFLOAT3A cameraPosition{ 0.0f, 0.0f, 0.0f };
	XMVECTOR cameraOrigin = XMVector3TransformCoord(XMLoadFloat3A(&cameraPosition), modelMatrix);
	XMVECTOR pickDirection = XMVector3Normalize(XMVector3TransformCoord(XMLoadFloat3A(&pickPosition), modelMatrix) - cameraOrigin);
	
	return m_mesh->GetOOBB().Intersects(cameraOrigin, pickDirection, distance);
	//return m_OOBB.Intersects(cameraOrigin, pickDirection, distance);
}

void CGameObject::Rotate(const float deltaTime)
{
	XMStoreFloat4x4A(&m_worldMatrix, XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3A(&m_totalRotation) * XMConvertToRadians(deltaTime * m_rotationSpeed)));
}

void CGameObject::Move(const float deltaTime)
{
	XMStoreFloat4x4A(&m_worldMatrix, XMLoadFloat4x4A(&m_worldMatrix) * XMMatrixTranslationFromVector(XMLoadFloat3A(&m_position)));
}

void CGameObject::Update(const float deltaTime)
{
	Rotate(deltaTime);
	Move(deltaTime);
	SetOOBB();

	//충돌 처리
}

void CGameObject::Render(HDC hDCFrameBuffer)
{
	if (!m_mesh)
		return;
	HPEN hPen = CreatePen(PS_SOLID, 0, m_color);
	HPEN hOldPen = (HPEN)SelectObject(hDCFrameBuffer, hPen);

	CGraphicsPipeline::SetWorldMatrix(m_worldMatrix);
	m_mesh->Render(hDCFrameBuffer);

	SelectObject(hDCFrameBuffer, hOldPen);
	DeleteObject(hPen);
}
