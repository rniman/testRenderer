#include "stdafx.h"
#include "GameObject.h"

CGameObject::CGameObject()
	: m_active{ true }
	, m_mesh{ nullptr }
	, m_color{ RGB(255, 0, 0) }
{
	XMStoreFloat4x4A(&m_worldMatrix, XMMatrixIdentity());
}

CGameObject::~CGameObject()
{
}

void CGameObject::SetPosition(const float x, const float y, const float z)
{
	m_worldMatrix._41 = x;
	m_worldMatrix._42 = y;
	m_worldMatrix._43 = z;
}

void CGameObject::SetPosition(const XMFLOAT3A& position)
{
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

void CGameObject::Move()
{
}

void CGameObject::Rotate(const float pitch, const float yaw, const float roll)
{
	XMStoreFloat4x4A(&m_worldMatrix, XMMatrixMultiply( XMMatrixRotationRollPitchYaw( XMConvertToRadians(pitch), XMConvertToRadians(yaw), XMConvertToRadians(roll)), XMLoadFloat4x4A(&m_worldMatrix)));
}

void CGameObject::Update(const float deltaTime)
{
	Rotate(0.0f, 0.0f, 50 * deltaTime);
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
