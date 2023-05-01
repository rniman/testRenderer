#include "stdafx.h"
#include "BulletObject.h"

CBulletObject::CBulletObject()
	: CGameObject()
	, m_elipsed{ 0.0f }
	, m_duration{ 5.0f }
	, m_damage{ 10 }
{
	m_active = false;
	m_color = RGB(200, 170, 100);

	m_rotationSpeed = 360.0f * 3;
	m_moveSpeed = 100.0f;

	m_pickingDetection = false;
}

CBulletObject::~CBulletObject()
{

}

UINT CBulletObject::GetDamge() const
{
	return m_damage;
}

void CBulletObject::SetActive()
{
	m_active = !m_active;
}

void CBulletObject::SetDamage(const float damage)
{
	m_damage = damage;
}

void CBulletObject::SetForward(const XMFLOAT3A& forward)
{
	XMStoreFloat3A(&m_forward, XMVector3Normalize(XMLoadFloat3A(&forward)));
}

void CBulletObject::DeleteBullet()
{
	SetActive();
	m_elipsed = 0.0f;
}

void CBulletObject::Rotate(const float deltaTime)
{
	XMMATRIX rotateMatrix = XMMatrixRotationAxis(XMLoadFloat3A(&m_forward), XMConvertToRadians(m_totalRotation.z * m_rotationSpeed));

	//XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3A(&m_totalRotation) * XMConvertToRadians(m_rotationSpeed));

	XMStoreFloat4x4A(&m_worldMatrix, rotateMatrix * XMMatrixTranslationFromVector(XMLoadFloat3A(&m_position)));
}

void CBulletObject::Move(const float deltaTime)
{
	XMStoreFloat4x4A(&m_worldMatrix, XMLoadFloat4x4A(&m_worldMatrix) * XMMatrixTranslationFromVector(XMLoadFloat3A(&m_forward) * m_moveSpeed * deltaTime));
	
	m_position.x = m_worldMatrix._41;
	m_position.y = m_worldMatrix._42;
	m_position.z = m_worldMatrix._43;
}

void CBulletObject::Update(const float deltaTime)
{
	if (!m_active)
	{
		return;
	}

	if (m_elipsed > m_duration)
	{
		DeleteBullet();
		return;
	}

	AddRotationAngle(0.0f, 0.0f, -deltaTime);
	
	Rotate(deltaTime);
	Move(deltaTime);
	SetOOBB();

	m_elipsed += deltaTime;
}

void CBulletObject::Render(HDC hDCFrameBuffer)
{
	CGameObject::Render(hDCFrameBuffer);
}
