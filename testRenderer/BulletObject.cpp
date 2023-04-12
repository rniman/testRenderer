#include "stdafx.h"
#include "BulletObject.h"

CBulletObject::CBulletObject()
	: CGameObject()
	, m_elipsed{ 0.0f }
	, m_duration{ 5.0f }
{
	m_active = false;
	m_color = RGB(200, 170, 100);

	m_rotationSpeed = 100.0f;
	m_moveSpeed = 10.0f;
}

CBulletObject::~CBulletObject()
{

}

void CBulletObject::SetActive()
{
	m_active = !m_active;
}

void CBulletObject::SetForward(const XMFLOAT3A& forward)
{
	XMStoreFloat3A(&m_forward, XMVector3Normalize(XMLoadFloat3A(&forward)));
}

void CBulletObject::Rotate(const float deltaTime)
{
	XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3A(&m_totalRotation) * XMConvertToRadians(m_rotationSpeed * deltaTime));

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
		SetActive();
		m_elipsed = 0.0f;
		return;
	}

	AddRotationAngle(0.0f, 0.0f, 1.0f);
	
	Rotate(deltaTime);
	Move(deltaTime);

	m_elipsed += deltaTime;
}

void CBulletObject::Render(HDC hDCFrameBuffer)
{
	CGameObject::Render(hDCFrameBuffer);
}