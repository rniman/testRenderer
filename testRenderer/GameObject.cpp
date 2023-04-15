#include "stdafx.h"
#include "GameObject.h"

CGameObject::CGameObject()
	: m_active{ true }
	, m_mesh{ nullptr }
	, m_color{ RGB(255, 0, 0) }
	, m_parent{ nullptr }
	, m_child{ nullptr }
	, m_sibling{ nullptr }
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
	delete m_child;
	m_child = nullptr;	

	delete m_sibling;
	m_sibling = nullptr;
}

CGameObject::CGameObject(CGameObject&& other) noexcept
{
	m_active = other.m_active;
	m_mesh = std::move(other.m_mesh);
	m_color = other.m_color;
	m_parent = other.m_parent;
	m_child = other.m_child;
	m_sibling = other.m_sibling;
	m_position = other.m_position;
	m_totalRotation = other.m_totalRotation;
	m_moveSpeed = other.m_moveSpeed;
	m_rotationSpeed = other.m_rotationSpeed;
	m_pickingDetection = other.m_pickingDetection;

	XMStoreFloat4x4A(&m_worldMatrix, XMLoadFloat4x4A(&other.m_worldMatrix));

	other.reset();
}

CGameObject& CGameObject::operator=(CGameObject&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}

	m_active = other.m_active;
	m_mesh = std::move(other.m_mesh);
	m_color = other.m_color;
	m_parent = other.m_parent;
	m_child = other.m_child;
	m_sibling = other.m_sibling;
	m_position = other.m_position;
	m_totalRotation = other.m_totalRotation;
	m_moveSpeed = other.m_moveSpeed;
	m_rotationSpeed = other.m_rotationSpeed;
	m_pickingDetection = other.m_pickingDetection;

	return *this;
}

bool CGameObject::GetActive() const
{
	return m_active;
}

bool CGameObject::GetPickingDetection() const
{
	return m_pickingDetection;
}

CGameObject* CGameObject::GetParent() const
{
	return m_parent;
}

CGameObject* CGameObject::GetChild() const
{
	return m_child;
}

CGameObject* CGameObject::GetSibling() const
{
	return m_sibling;
}

XMFLOAT4X4A CGameObject::GetWorldMatrix() const
{
	return m_worldMatrix;
}

XMFLOAT3A CGameObject::GetPosition() const
{
	return m_position;
}

XMFLOAT3A CGameObject::GetTotalRotation() const
{
	return m_totalRotation;
}

BoundingOrientedBox CGameObject::GetOOBB() const
{
	return m_OOBB;
}

void CGameObject::SetMesh(const std::shared_ptr<CMesh>& mesh)
{
	m_mesh = mesh;
}

DWORD CGameObject::SetColor(const DWORD color)
{
	DWORD oldColor = m_color;
	m_color = color;
	return oldColor;
}

void CGameObject::SetParent(CGameObject& parent)
{
	m_parent = &parent;
}

void CGameObject::SetChild(CGameObject& child)
{
	m_child = &child;
}

void CGameObject::SetSibling(CGameObject& sibling)
{
	m_sibling = &sibling;
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

void CGameObject::SetRotationSpeed(const float rotationSpeed)
{
	m_rotationSpeed = rotationSpeed;
}

void CGameObject::SetMoveSpeed(const float moveSpeed)
{
	m_moveSpeed = moveSpeed;
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

	m_position.x = m_worldMatrix._41;
	m_position.y = m_worldMatrix._42;
	m_position.z = m_worldMatrix._43;
}

void CGameObject::Update(const float deltaTime)
{
	Rotate(deltaTime);
	Move(deltaTime);
	SetOOBB();

	if (m_parent)
	{
		XMFLOAT4X4A parentWorld = m_parent->GetWorldMatrix();
		XMStoreFloat4x4A(&m_worldMatrix, XMLoadFloat4x4A(&m_worldMatrix) * XMLoadFloat4x4A(&parentWorld));
	}

	if (m_sibling)
	{
		m_sibling->Update(deltaTime);
	}
	if (m_child)
	{
		m_child->Update(deltaTime);
	}

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

	if (m_child) m_child->Render(hDCFrameBuffer);
	if (m_sibling) m_sibling->Render(hDCFrameBuffer);

	SelectObject(hDCFrameBuffer, hOldPen);
	DeleteObject(hPen);
}

void CGameObject::reset()
{
	m_active = true;
	m_mesh = nullptr;
	m_color = RGB(255, 0, 0);
	m_parent = nullptr;
	m_child = nullptr;
	m_sibling = nullptr;
	m_position = { 0.0f, 0.0f, 0.0f };
	m_totalRotation = { 0.0f, 0.0f, 0.0f };
	m_moveSpeed = 0.0f;
	m_rotationSpeed = 0.0f;
	m_pickingDetection = true;
}
