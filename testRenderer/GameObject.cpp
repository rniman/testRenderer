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

void CGameObject::AddRotationAngle(const float pitch, const float yaw, const float roll)
{
	AddRotationAngle(XMFLOAT3A(pitch, yaw, roll));
}

void CGameObject::AddRotationAngle(const XMFLOAT3A& rotate)
{
	XMStoreFloat3A(&m_totalRotation, XMLoadFloat3A(&m_totalRotation) + XMLoadFloat3(&rotate));
}

void CGameObject::Rotate(const float deltaTime)
{
	XMStoreFloat4x4A(&m_worldMatrix, XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3A(&m_totalRotation) * deltaTime * m_rotationSpeed));
}

void CGameObject::Move(const float deltaTime)
{
	XMStoreFloat4x4A(&m_worldMatrix, XMLoadFloat4x4A(&m_worldMatrix) * XMMatrixTranslationFromVector(XMLoadFloat3A(&m_position)));
}

void CGameObject::Update(const float deltaTime)
{
	Rotate(deltaTime);
	Move(deltaTime);
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
