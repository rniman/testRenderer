#include "stdafx.h"
#include "EenemyObject.h"

CEenemyObject::CEenemyObject()
	: CGameObject()	
	, m_hp{ 0 }
{
	m_pickingDetection = true;
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
	, m_look{}
	, m_target{ nullptr }
	, m_targetDistance{}
	, m_moveDirection{}
	, m_searchTime{}
	, m_bullet(MAX_BULLET)
	, m_coolTime{ 0.0f }
{
	m_hp = 50;

	m_rotationSpeed = 90.0f;
	m_moveSpeed = 10.0f;

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

void CEnemyTank::SetMove(bool bMove)
{
	m_bMove = bMove;
}

void CEnemyTank::SetTarget(CGameObject* target)
{
	m_target = target;
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

void CEnemyTank::FindTarget()
{
	if (!m_target)
	{
		return;
	}

	XMFLOAT3A pos = m_position;
	pos.y = 1.0f;
	XMVECTOR position = XMLoadFloat3A(&pos);	

	XMFLOAT3A targetPosition = m_target->GetPosition();
	XMVECTOR targetPos = XMLoadFloat3A(&targetPosition);

	XMVECTOR target = targetPos - position;
	XMVECTOR distance = XMVector3Length(target);
	XMVECTOR targetDirection = XMVector3Normalize(target);

	m_targetDistance = XMVectorGetX(distance);
	XMStoreFloat3A(&m_moveDirection, targetDirection);

	if (m_bMove)
	{
		if (m_targetDistance >= 120.0f)
		{
			m_bLockTarget = false;
			return;
		}
	}
	else
	{
		if (m_targetDistance >= 140.0f)
		{
			m_bLockTarget = false;
			return;
		}
	}

	if ((!m_bMove && m_targetDistance <= 140.0f) || m_targetDistance <= 60.0f || m_collidedObject)
	{
		XMFLOAT3A turretRotation = m_turret->GetTotalRotation();
		XMMATRIX rotate = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3A(&turretRotation) * XMConvertToRadians(1.0f));

		XMVECTOR look = XMVector3TransformNormal(XMLoadFloat3A(&m_look), rotate);
		XMVECTOR direction = XMLoadFloat3A(&m_moveDirection);

		float angle = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenNormals(look, direction)));
		if (XMVectorGetY(XMVector3Cross(look, direction)) < 0.0f)
		{
			angle = -angle;
		}
		m_remainTurretRotation = angle;

		if (m_bMove)
		{
			m_bLockTarget = true;
			return;
		}

		position = XMLoadFloat3A(&m_position);
		targetPos = XMLoadFloat3A(&targetPosition);
		XMFLOAT3A targetXZ = targetPosition;
		targetXZ.y = 11.0f;

		angle = -XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenVectors(XMLoadFloat3A(&targetXZ) - position, targetPos - position)));

		XMFLOAT3A gunRotation = m_gun->GetTotalRotation();
		m_remainGunRotation = angle + gunRotation.x;
	}
	else if (m_targetDistance >= 60.0f)
	{
		XMVECTOR look = XMLoadFloat3A(&m_look);
		XMVECTOR direction = XMLoadFloat3A(&m_moveDirection);
		float angle = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenNormals(look, direction)));
		if (XMVectorGetY(XMVector3Cross(look, direction)) < 0.0f)
		{
			angle = -angle;
		}
		m_remainRotation = angle;
		m_remainTurretRotation = -m_turret->GetTotalRotation().y;

	}
	m_bLockTarget = true;
}

void CEnemyTank::RotateToTarget(const float deltaTime)
{
	m_oldTotalRotation = m_totalRotation;

	if (m_remainRotation && m_bMove)
	{
		if (abs(m_remainRotation) <= m_rotationSpeed * deltaTime)
		{
			m_totalRotation.y += m_remainRotation;
			m_remainRotation = 0.0f;
		}
		else
		{
			if (m_remainRotation >= 0.0f)
			{
				m_totalRotation.y += m_rotationSpeed * deltaTime;
				m_remainRotation -= m_rotationSpeed * deltaTime;
			}
			else
			{
				m_totalRotation.y -= m_rotationSpeed * deltaTime;
				m_remainRotation += m_rotationSpeed * deltaTime;
			}
		}
	}

	if (m_remainTurretRotation)
	{
		if (abs(m_remainTurretRotation) <= m_rotationSpeed * deltaTime)
		{
			m_turret->AddRotationAngle(0.0f, m_remainTurretRotation, 0.0f);
			m_remainTurretRotation = 0.0f;
		}
		else
		{
			if (m_remainTurretRotation >= 0.0f)
			{
				m_turret->AddRotationAngle(0.0f, m_rotationSpeed * deltaTime, 0.0f);
				m_remainTurretRotation -= m_rotationSpeed * deltaTime;
			}
			else
			{
				m_turret->AddRotationAngle(0.0f, -m_rotationSpeed * deltaTime, 0.0f);
				m_remainTurretRotation += m_rotationSpeed * deltaTime;
			}
		}
	}
	
	if (m_remainGunRotation)
	{
		if (abs(m_remainGunRotation) <= deltaTime * m_rotationSpeed)
		{
			m_gun->AddRotationAngle(-m_remainGunRotation, 0.0f, 0.0f);
			m_remainGunRotation = 0.0f;
		}
		else if (m_remainGunRotation >= 0.0f)
		{
			m_gun->AddRotationAngle(-deltaTime * m_rotationSpeed, 0.0f, 0.0f);
			m_remainGunRotation -= deltaTime * m_rotationSpeed;
		}
		else if (m_remainGunRotation < 0.0f)
		{
			m_gun->AddRotationAngle(deltaTime * m_rotationSpeed, 0.0f, 0.0f);
			m_remainGunRotation += deltaTime * m_rotationSpeed;

		}
	}
}

void CEnemyTank::Rotate(const float deltaTime)
{
	XMStoreFloat4x4A(&m_worldMatrix, XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3A(&m_totalRotation) * XMConvertToRadians(1.0f)));

	m_look.x = m_worldMatrix._31;
	m_look.y = m_worldMatrix._32;
	m_look.z = m_worldMatrix._33;

	XMStoreFloat3A(&m_look, XMVector3Normalize(XMLoadFloat3A(&m_look)));
}

void CEnemyTank::Move(const float deltaTime)
{
	m_oldPosition = m_position;
	
	if (m_targetDistance >= 120.0f || !m_bMove)
	{
		XMStoreFloat4x4A(&m_worldMatrix, XMLoadFloat4x4A(&m_worldMatrix) * XMMatrixTranslationFromVector(XMLoadFloat3A(&m_position)));

		m_position.x = m_worldMatrix._41;
		m_position.y = m_worldMatrix._42;
		m_position.z = m_worldMatrix._43;
		
		return;
	}

	if (m_targetDistance >= 60.0f)
	{
		XMStoreFloat3A(&m_position, XMVectorAdd(XMLoadFloat3A(&m_position), XMLoadFloat3A(&m_moveDirection) * m_moveSpeed * deltaTime));
	}

	XMStoreFloat4x4A(&m_worldMatrix, XMLoadFloat4x4A(&m_worldMatrix) * XMMatrixTranslationFromVector(XMLoadFloat3A(&m_position)));

	m_position.x = m_worldMatrix._41;
	m_position.y = m_worldMatrix._42;
	m_position.z = m_worldMatrix._43;
}

void CEnemyTank::Collide(const float deltaTime)
{
	if (!m_active)
	{
		return;
	}

	if (dynamic_cast<CBulletObject*>(m_collidedObject))
	{
		m_hp -= static_cast<CBulletObject*>(m_collidedObject)->GetDamge();
		if (m_hp <= 0)
		{
			m_active = false;
		}
		return;
	}

	m_position = m_oldPosition;
	m_totalRotation = m_oldTotalRotation;

	Rotate(deltaTime);
	XMStoreFloat4x4A(&m_worldMatrix, XMLoadFloat4x4A(&m_worldMatrix) * XMMatrixTranslationFromVector(XMLoadFloat3A(&m_position)));
	SetOOBB();
}

void CEnemyTank::Update(const float deltaTime)
{
	if (m_static)
	{
		return;
	}

	RotateToTarget(deltaTime);

	Rotate(deltaTime);
	Move(deltaTime);
	if (m_parent)
	{
		XMFLOAT4X4A parentWorld = m_parent->GetWorldMatrix();
		XMStoreFloat4x4A(&m_worldMatrix, XMLoadFloat4x4A(&m_worldMatrix) * XMLoadFloat4x4A(&parentWorld));
	}
	SetOOBB();
	

	if (m_sibling)
	{
		m_sibling->Update(deltaTime);
	}
	if (m_child)
	{
		m_child->Update(deltaTime);
	}
	
	if (m_coolTime > 0.0f)
	{
		m_coolTime -= deltaTime;
	}
	if(m_bLockTarget)FireBullet();

	for (CBulletObject& bullet : m_bullet)
	{
		if (!bullet.GetActive())
		{
			continue;
		}

		bullet.Update(deltaTime);
	}

	m_searchTime -= deltaTime;
	if (m_searchTime <= 0.0f)
	{
		m_searchTime = 1.5f;
		//À§Ä¡ Å½»ö
		FindTarget();
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
