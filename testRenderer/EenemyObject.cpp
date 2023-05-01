#include "stdafx.h"
#include "EenemyObject.h"

CEenemyObject::CEenemyObject()
	: CGameObject()	
	, m_hp{ 0 }
{
}

CEenemyObject::~CEenemyObject()
{
}

UINT CEenemyObject::GetHP() const
{
	return m_hp;
}

void CEenemyObject::AddRotationAngle(const float pitch, const float yaw, const float roll)
{
	CGameObject::AddRotationAngle(pitch, yaw, roll);
}

void CEenemyObject::AddRotationAngle(const XMFLOAT3A& rotate)
{
	CGameObject::AddRotationAngle(rotate);
}

void CEenemyObject::Rotate(const float deltaTime)
{
	CGameObject::Rotate(deltaTime);
}

void CEenemyObject::Move(const float deltaTime)
{
	CGameObject::Move(deltaTime);
}

void CEenemyObject::Update(const float deltaTime)
{
	if (m_hp <= 0)
	{
		m_active = false;
		return;
	}

	CGameObject::Update(deltaTime);
}

void CEenemyObject::Collide(const float deltaTima)
{
}

void CEenemyObject::Render(HDC hDCFrameBuffer)
{
	CGameObject::Render(hDCFrameBuffer);
}

/// <CEenemyObject>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CEnemyTank>

CEnemyTank::CEnemyTank()
	: CEenemyObject()
	, m_bullet(MAX_BULLET)
	, m_coolTime{ 0.0f }
{
	m_hp = 100;

	m_rotationSpeed = 90.0f;
	m_moveSpeed = 0.0f;

	m_turret = new CGameObject();
	m_child = m_turret;
	m_turret->SetParent(*this);
	m_turret->SetRotationSpeed(m_rotationSpeed);

	m_gun = new CGameObject();
	m_child->SetChild(*m_gun);
	m_gun->SetParent(*m_child);
	m_gun->SetRotationSpeed(m_rotationSpeed);

	std::shared_ptr<CMesh> bulletMesh = std::make_shared<CCube>(1.0f, 1.0f, 1.0f);
	for (CBulletObject& bullet : m_bullet)
	{
		bullet.SetMesh(bulletMesh);
	}
}

CEnemyTank::~CEnemyTank()
{
	m_turret = nullptr;
	m_gun = nullptr;
}

CGameObject* CEnemyTank::GetTurret() const
{
	return m_turret;
}

CGameObject* CEnemyTank::GetGun() const
{
	return m_gun;
}

std::vector<CBulletObject>& CEnemyTank::GetBullets()
{
	return m_bullet;
}

void CEnemyTank::AddRotationAngle(const float pitch, const float yaw, const float roll)
{
	CEenemyObject::AddRotationAngle(pitch, yaw, roll);
}

void CEnemyTank::AddRotationAngle(const XMFLOAT3A& rotate)
{
	CEenemyObject::AddRotationAngle(rotate);
}

void CEnemyTank::FireBullet()
{
	if (m_coolTime > 0.0f)
	{
		return;
	}

	for (CBulletObject& bullet : m_bullet)
	{
		if (bullet.GetActive())
		{
			continue;
		}

		bullet.SetActive();

		XMFLOAT3A firePosition = m_gun->GetPosition();
		XMFLOAT4X4A gunParentWorld = m_gun->GetParent()->GetWorldMatrix();
		XMStoreFloat3A(&firePosition, XMVector3TransformCoord(XMLoadFloat3(&firePosition), XMLoadFloat4x4A(&gunParentWorld)));
		bullet.SetPosition(firePosition);

		XMFLOAT3A fireDirection = { 0.0f, 0.0f, 1.0f };
		XMFLOAT4X4A gunMatrix = m_gun->GetWorldMatrix();

		XMStoreFloat3A(&fireDirection, XMVector3TransformNormal(XMLoadFloat3A(&fireDirection), XMLoadFloat4x4A(&gunMatrix)));

		bullet.SetForward(fireDirection);
		m_coolTime = 1.0f;
		break;
	}
}

void CEnemyTank::Rotate(const float deltaTime)
{
	XMStoreFloat4x4A(&m_worldMatrix, XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3A(&m_totalRotation) * XMConvertToRadians(1.0f)));
}

void CEnemyTank::Move(const float deltaTime)
{
	CEenemyObject::Move(deltaTime);
}

void CEnemyTank::Collide(const float deltaTime)
{
	if (dynamic_cast<CBulletObject*>(m_collidedObject))
	{
		m_hp -= static_cast<CBulletObject*>(m_collidedObject)->GetDamge();
		if (m_hp <= 0)
		{
			m_active = false;
		}
	}
}

void CEnemyTank::Update(const float deltaTime)
{
	CEenemyObject::Update(deltaTime);
	
	if (m_coolTime > 0.0f)
	{
		m_coolTime -= deltaTime;
	}
	FireBullet();


	for (CBulletObject& bullet : m_bullet)
	{
		if (!bullet.GetActive())
		{
			continue;
		}

		bullet.Update(deltaTime);
	}
}

void CEnemyTank::Render(HDC hDCFrameBuffer)
{
	if (!m_mesh)
		return;
	HPEN hPen = CreatePen(PS_SOLID, 0, m_color);
	HPEN hOldPen = (HPEN)SelectObject(hDCFrameBuffer, hPen);

	CGraphicsPipeline::SetWorldMatrix(m_worldMatrix);
	m_mesh->Render(hDCFrameBuffer);

	if (m_child) m_child->Render(hDCFrameBuffer);
	if (m_sibling) m_sibling->Render(hDCFrameBuffer);

	for (CBulletObject& bullet : m_bullet)
	{
		if (!bullet.GetActive())
		{
			continue;
		}

		bullet.Render(hDCFrameBuffer);
	}

	SelectObject(hDCFrameBuffer, hOldPen);
	DeleteObject(hPen);
}
