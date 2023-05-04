#include "stdafx.h"
#include "Player.h"

CPlayer::CPlayer()
	: CGameObject()
	, m_right{ XMFLOAT3A(1.0f,0.0f, 0.0f) }
	, m_up{ XMFLOAT3A(0.0f,1.0f, 0.0f) }
	, m_look{ XMFLOAT3A(0.0f,0.0f, 1.0f) }
	, m_camera()
	, m_bMoveForce{ false }
	, m_moveDirection{ 0.0f, 0.0f, 0.0f }
	, m_acceleration{ 60.0f }
	, m_friction{ 45.0f }
	, m_cameraRotation{ 0.0f, 0.0f, 0.0f }
{
	m_rotationSpeed = 5.0f;
	m_moveSpeed = 0.0f;
}

CPlayer::CPlayer(const CCamera& camera)
	: CGameObject()
	, m_right{ XMFLOAT3A(1.0f,0.0f, 0.0f) }
	, m_up{ XMFLOAT3A(0.0f,1.0f, 0.0f) }
	, m_look{ XMFLOAT3A(0.0f,0.0f, 1.0f) }
	, m_camera(camera)
	, m_bMoveForce{ false }
	, m_moveDirection{ 0.0f, 0.0f, 0.0f }
	, m_acceleration{ 60.0f }
	, m_friction{ 45.0f }
	, m_cameraRotation{ 0.0f, 0.0f, 0.0f }
{
	m_rotationSpeed = 5.0f;
	m_moveSpeed = 0.0f;
}

CPlayer::~CPlayer()
{
}

CCamera CPlayer::GetCamera() const
{
	return m_camera;
}

void CPlayer::SetCamera(const CCamera& camera)
{
	m_camera = camera;
}

void CPlayer::SetDirection()
{
	m_moveDirection = XMFLOAT3A();
}

void CPlayer::SetInactiveMoveForce()
{
	m_bMoveForce = false;
}

void CPlayer::AddRotationAngle(const float pitch, const float yaw, const float roll)
{
	XMFLOAT3A angle = {pitch, yaw * m_rotationSpeed, roll};
	XMStoreFloat3A(&m_totalRotation, XMLoadFloat3A(&m_totalRotation) + XMLoadFloat3(&angle));

	if (m_totalRotation.y > 180.0f)
	{
		m_totalRotation.y = -360.0f + m_totalRotation.y;
	}
	else if (m_totalRotation.y < -180.0f)
	{
		m_totalRotation.y = 360.0f + m_totalRotation.y;
	}

}

void CPlayer::AddCameraRotation(const float pitch, const float yaw, const float roll)
{
	m_cameraRotation.x += pitch;
	m_cameraRotation.y += yaw;
	m_cameraRotation.z += roll;
}

void CPlayer::Rotate(const float deltaTime)
{
	//회전행렬 구하고
	XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3A(&m_totalRotation) * XMConvertToRadians(1.0f));

	XMStoreFloat4x4A(&m_worldMatrix, rotateMatrix * XMMatrixTranslationFromVector(XMLoadFloat3A(&m_position)));

	XMVECTOR look = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	look = XMVector3TransformNormal(look, XMLoadFloat4x4A(&m_worldMatrix));
	XMStoreFloat3A(&m_look, look);

	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	up = XMVector3TransformNormal(up, XMLoadFloat4x4A(&m_worldMatrix));
	XMStoreFloat3A(&m_up, up);

	XMVECTOR right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	right = XMVector3TransformNormal(right, XMLoadFloat4x4A(&m_worldMatrix));
	XMStoreFloat3A(&m_right, right);
}

void CPlayer::Move(const float deltaTime)
{
	if (m_bMoveForce)
	{
		if (m_moveSpeed < 15.0f)
		{
			m_moveSpeed += deltaTime * (m_acceleration - m_friction);
			if (m_moveSpeed > 15.0f)
			{
				m_moveSpeed = 15.0f;
			}
		}
	}
	else
	{
		if (m_moveSpeed > 0.0f)
		{
			m_moveSpeed -= deltaTime * m_friction;
			if (m_moveSpeed < 0.0f)
			{
				m_moveSpeed = 0.0f;
			}
		}
	}

	XMStoreFloat4x4A(&m_worldMatrix, XMLoadFloat4x4A(&m_worldMatrix) * XMMatrixTranslationFromVector(XMLoadFloat3A(&m_moveDirection) * m_moveSpeed * deltaTime));
	
	m_position.x = m_worldMatrix._41;
	m_position.y = m_worldMatrix._42;
	m_position.z = m_worldMatrix._43;
}

void CPlayer::HandleInput(DWORD direction)
{
	if (direction)
	{
		SetDirection();
		m_bMoveForce = true;
		if (direction & DIR_FORWARD) XMStoreFloat3A(&m_moveDirection, XMLoadFloat3A(&m_moveDirection) + XMLoadFloat3A(&m_look));
		if (direction & DIR_BACKWARD) XMStoreFloat3A(&m_moveDirection, XMLoadFloat3A(&m_moveDirection) - XMLoadFloat3A(&m_look));
		if (direction & DIR_RIGHT) XMStoreFloat3A(&m_moveDirection, XMLoadFloat3A(&m_moveDirection) + XMLoadFloat3A(&m_right));
		if (direction & DIR_LEFT) XMStoreFloat3A(&m_moveDirection, XMLoadFloat3A(&m_moveDirection) - XMLoadFloat3A(&m_right));
		//if (direction & DIR_UP) XMStoreFloat3A(&m_moveDirection, XMLoadFloat3A(&m_moveDirection) + XMLoadFloat3A(&m_up));
		//if (direction & DIR_DOWN) XMStoreFloat3A(&m_moveDirection, XMLoadFloat3A(&m_moveDirection) - XMLoadFloat3A(&m_up));
		XMStoreFloat3A(&m_moveDirection, XMVector3Normalize(XMLoadFloat3A(&m_moveDirection)));
	}
}

void CPlayer::Update(const float deltaTime)
{
	//행렬을 업데이트하고 움직인다//
	Rotate(deltaTime);
	Move(deltaTime);

	SetOOBB();
	
	m_camera.SetLookTo(m_position, m_look);
	m_camera.Update(deltaTime);

	if (m_sibling)
	{
		m_sibling->Update(deltaTime);
	}
	if (m_child)
	{
		m_child->Update(deltaTime);
	}
}

void CPlayer::Collide(const float deltaTime)
{
	
}

void CPlayer::Render(HDC hDCFrameBuffer)
{
}

/// <CPlayer>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CTankPlayer>

CTankPlayer::CTankPlayer()
	: CPlayer()
	, m_cameraOffset{ 0.0f, 10.0f, -30.0f }
	, m_bMainCamera{ true }
	, m_pickingObject{ nullptr }
	, m_turret{ nullptr }
	, m_gun{ nullptr }
	, m_bullet(MAX_BULLET)
	, m_coolTime{ 0.0f }
	, m_hp{ 150 }
{
	SetPosition(XMFLOAT3A(0.0f, 1.0f, 0.0f));
	m_oldPosition = m_position;
	m_camera.SetPosition(m_cameraOffset);

	m_turret = new CGameObject();
	m_child = m_turret;
	m_turret->SetParent(*this);
	m_turret->SetRotationSpeed(30.0f);

	m_gun = new CGameObject();
	m_child->SetChild(*m_gun);
	m_gun->SetParent(*m_child);
	m_gun->SetRotationSpeed(30.0f);

	std::shared_ptr<CMesh> bulletMesh = std::make_shared<CCube>(2.0f, 2.0f, 2.0f);
	for (CBulletObject& bullet : m_bullet)
	{
		bullet.SetMesh(bulletMesh);
	}
}

CTankPlayer::CTankPlayer(const CCamera& camera)
	: CPlayer(camera)
	, m_cameraOffset{ 0.0f, 10.0f, -30.0f }
	, m_bMainCamera{ true }
	, m_pickingObject{ nullptr }
	, m_turret{ nullptr }
	, m_gun{ nullptr }
	, m_bullet(MAX_BULLET)
	, m_coolTime{ 0.0f }
	, m_hp{ 150 }
{
	SetPosition(XMFLOAT3A(0.0f, 1.0f, 0.0f));
	m_oldPosition = m_position;
	m_camera.SetPosition(m_cameraOffset);
	
	m_turret = new CGameObject();
	m_child = m_turret;
	m_turret->SetParent(*this);
	m_turret->SetRotationSpeed(30.0f);

	m_gun = new CGameObject();
	m_child->SetChild(*m_gun);
	m_gun->SetParent(*m_child);
	m_gun->SetRotationSpeed(30.0f);

	std::shared_ptr<CMesh> bulletMesh = std::make_shared<CCube>(1.0f, 1.0f, 1.0f);
	for (CBulletObject& bullet : m_bullet)
	{
		bullet.SetMesh(bulletMesh);
	}
}

CTankPlayer::~CTankPlayer() 
{
	m_turret = nullptr;
	m_gun = nullptr;
}

CGameObject* CTankPlayer::GetTurret() const
{
	return m_turret;
}

CGameObject* CTankPlayer::GetGun() const
{
	return m_gun;
}

UINT CTankPlayer::GetHp() const
{
	return m_hp;
}

std::vector<CBulletObject>& CTankPlayer::GetBullets()
{
	return m_bullet;
}

void CTankPlayer::SetPicking(CGameObject* pickingObject)
{
	m_pickingObject = pickingObject;
}

void CTankPlayer::AddRotationAngle(const float pitch, const float yaw, const float roll)
{
	m_oldTotalRotation = m_totalRotation;
	CPlayer::AddRotationAngle(pitch, yaw, roll);
	m_bMainCamera = true;
}

void CTankPlayer::AddCameraRotation(const float pitch, const float yaw, const float roll)
{
	CPlayer::AddCameraRotation(pitch, yaw, roll);
	m_bMainCamera = false;
}

void CTankPlayer::FireBullet()
{
	if (m_coolTime > 0.0f)
	{
		return;
	}

	for (CBulletObject& bullet: m_bullet)
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

void CTankPlayer::AimTarget()
{
	m_remainTurretRotation = 0.0f;
	m_remainGunRotation = 0.0f;

	XMFLOAT3A targetPos = m_pickingObject->GetPosition();
	XMFLOAT3A targetXZ = targetPos;
	targetXZ.y = 1.0f;

	XMVECTOR targetPosition = XMLoadFloat3A(&targetXZ);
	XMVECTOR position = XMLoadFloat3A(&m_position);

	XMVECTOR directionXZ = XMVector3Normalize(targetPosition - position);
	
	XMFLOAT3A turretRotation = m_turret->GetTotalRotation();
	XMMATRIX rotate = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3A(&turretRotation) * XMConvertToRadians(1.0f));
	XMVECTOR turretLook = XMVector3TransformNormal(XMLoadFloat3A(&m_look), rotate);

	float angle = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenNormals(turretLook, directionXZ)));
	if (XMVectorGetY(XMVector3Cross(turretLook, directionXZ)) < 0.0f)
	{
		angle = -angle;
	}
	m_remainTurretRotation = angle;

	targetPosition = XMLoadFloat3A(&targetPos);
	angle = XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenVectors(XMLoadFloat3A(&targetXZ) - position, targetPosition - position)));
	
	XMFLOAT3A gunRotation = m_gun->GetTotalRotation();
	m_remainGunRotation = angle + gunRotation.x;
	if (abs(m_remainGunRotation) <= 1.0f)
	{
		m_remainGunRotation = 0.0f;
	}
}

void CTankPlayer::HandleInput(DWORD direction)
{
	if (direction)
	{
		m_bMainCamera = true;
		SetDirection();
		m_bMoveForce = true;
		if (direction & DIR_FORWARD) XMStoreFloat3A(&m_moveDirection, XMLoadFloat3A(&m_moveDirection) + XMLoadFloat3A(&m_look));
		if (direction & DIR_BACKWARD) XMStoreFloat3A(&m_moveDirection, XMLoadFloat3A(&m_moveDirection) - XMLoadFloat3A(&m_look));
		if (direction & DIR_RIGHT) XMStoreFloat3A(&m_moveDirection, XMLoadFloat3A(&m_moveDirection) + XMLoadFloat3A(&m_right));
		if (direction & DIR_LEFT) XMStoreFloat3A(&m_moveDirection, XMLoadFloat3A(&m_moveDirection) - XMLoadFloat3A(&m_right));

		XMStoreFloat3A(&m_moveDirection, XMVector3Normalize(XMLoadFloat3A(&m_moveDirection)));
	}
}

void CTankPlayer::Collide(const float deltaTime)
{
	if (dynamic_cast<CBulletObject*>(m_collidedObject))
	{
		m_hp -= static_cast<CBulletObject*>(m_collidedObject)->GetDamge();
		if (m_hp <= 0)
		{
			m_active = false;
		}
		return;
	}

	m_moveSpeed = 0.0f;
	m_position = m_oldPosition;
	m_totalRotation = m_oldTotalRotation;

	Rotate(deltaTime);
	SetOOBB();
	
	m_turret->Update(deltaTime);

	m_camera.BackCameraPosition();
	m_camera.SetLookAt(m_camera.GetPosition(), m_position, m_up);
	m_camera.SetCameraProjectMatrix();
	m_camera.SetFrustumWorld();
}

void CTankPlayer::Rotate(const float deltaTime)
{
	if (abs(m_remainTurretRotation) <= m_turret->GetRotateSpeed() * deltaTime)
	{
		m_turret->AddRotationAngle(0.0f, m_remainTurretRotation, 0.0f);
	}
	else if (m_remainTurretRotation > 0.0f)
	{
		m_turret->AddRotationAngle(0.0f, deltaTime * m_turret->GetRotateSpeed(), 0.0f);
	}
	else if (m_remainTurretRotation < 0.0f)
	{
		m_turret->AddRotationAngle(0.0f, -deltaTime * m_turret->GetRotateSpeed(), 0.0f);	
	}	

	if (abs(m_remainGunRotation) <= deltaTime * m_gun->GetRotateSpeed())
	{
		m_gun->AddRotationAngle(m_remainGunRotation, 0.0f, 0.0f);
	}
	else if (m_remainGunRotation > 0.0f)
	{
		m_gun->AddRotationAngle(-deltaTime * m_gun->GetRotateSpeed(), 0.0f, 0.0f);
	}
	else if (m_remainGunRotation < 0.0f)
	{
		m_gun->AddRotationAngle(deltaTime * m_gun->GetRotateSpeed(), 0.0f, 0.0f);
	}

	CPlayer::Rotate(deltaTime);
}

void CTankPlayer::Move(const float deltaTime)
{
	if (m_bMoveForce)
	{
		if (m_moveSpeed < 15.0f)
		{
			m_moveSpeed += deltaTime * (m_acceleration - m_friction);
			if (m_moveSpeed > 15.0f)
			{
				m_moveSpeed = 15.0f;
			}
		}
	}
	else
	{
		if (m_moveSpeed > 0.0f)
		{
			m_moveSpeed -= deltaTime * m_friction;
			if (m_moveSpeed < 0.0f)
			{
				m_moveSpeed = 0.0f;
			}
		}
	}

	XMStoreFloat4x4A(&m_worldMatrix, XMLoadFloat4x4A(&m_worldMatrix) * XMMatrixTranslationFromVector(XMLoadFloat3A(&m_moveDirection) * m_moveSpeed * deltaTime));

	if (!XMVector3Equal(XMLoadFloat3A(&m_oldPosition), XMLoadFloat3A(&m_position)))
	{
		m_oldPosition = m_position;
	}

	m_position.x = m_worldMatrix._41;
	m_position.y = m_worldMatrix._42;
	m_position.z = m_worldMatrix._43;
}

void CTankPlayer::Update(const float deltaTime)
{
	if (!m_active)
	{
		return;
	}

	if (m_pickingObject)
	{
		if (!m_pickingObject->GetActive())
		{
			m_pickingObject = nullptr;
		}
		else
		{
			AimTarget();
		}
	}
	else
	{
		m_remainTurretRotation = 0.0f;
		m_remainGunRotation = 0.0f;
	}

	Rotate(deltaTime);
	Move(deltaTime);
	SetOOBB();

	XMFLOAT3A eye;
	if (m_bMainCamera)
	{
		if (m_cameraRotation.x || m_cameraRotation.y || m_cameraRotation.z)
		{
			XMVECTOR p0, p1;
			XMVECTOR axis;
			XMFLOAT3A cameraPosition = m_camera.GetPosition();
			p0 = XMVector3TransformCoord(XMLoadFloat3A(&m_cameraOffset), XMLoadFloat4x4A(&m_worldMatrix)) - XMLoadFloat3A(&m_position);
			p1 = XMLoadFloat3A(&cameraPosition) - XMLoadFloat3A(&m_position);
			
			XMFLOAT3A rotateDirection;
			XMStoreFloat3A(&rotateDirection,XMVector3Normalize(XMVector3Cross(p0, p1)));
			
			XMVECTOR angle;
			angle = XMVector3AngleBetweenVectors(p0, p1);
			float angleDegree = XMConvertToDegrees(XMVectorGetX(angle));

			if (rotateDirection.y < 0.0f)
			{
				axis = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
			}
			else
			{
				axis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			}

			m_elipsedRotation += deltaTime;
			XMMATRIX rotate;
			rotate = XMMatrixRotationAxis(axis, XMConvertToRadians(-180.0f) * m_elipsedRotation);

			if (angleDegree <= 5.0f)
			{
				XMStoreFloat3A(&eye, XMVector3TransformCoord(XMLoadFloat3A(&m_cameraOffset), XMLoadFloat4x4A(&m_worldMatrix)));
				XMFLOAT3A look;
				XMStoreFloat3(&look, XMVector3Normalize(XMVectorSubtract(XMLoadFloat3A(&m_position), XMLoadFloat3(&eye))));
				m_camera.SetLookTo(eye, look, m_up);

				XMStoreFloat3A(&m_cameraRotation, XMVectorZero());
				m_elipsedRotation = 0.0f;
			}
			else
			{
				XMStoreFloat3A(&eye, XMVector3TransformCoord(XMLoadFloat3A(&m_cameraOffset), XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3A(&m_totalRotation) * XMConvertToRadians(m_rotationSpeed * deltaTime))));
				XMStoreFloat3A(&eye, XMVector3TransformCoord(XMLoadFloat3A(&eye), XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3A(&m_cameraRotation) * XMConvertToRadians(m_rotationSpeed * deltaTime))));
				XMStoreFloat3A(&eye, XMVector3TransformCoord(XMLoadFloat3A(&eye), rotate));
				XMStoreFloat3A(&eye, XMVector3TransformCoord(XMLoadFloat3A(&eye), XMMatrixTranslationFromVector(XMLoadFloat3A(&m_position))));

				m_camera.SetLookAt(eye, m_position, m_up);
			}
		}
		else
		{
			XMStoreFloat3A(&eye, XMVector3TransformCoord(XMLoadFloat3A(&m_cameraOffset), XMLoadFloat4x4A(&m_worldMatrix)));
			m_camera.SetLookAt(eye, m_position, m_up);
		}
	}
	else
	{
		XMStoreFloat3A(&eye, XMVector3TransformCoord(XMLoadFloat3A(&m_cameraOffset), XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3A(&m_totalRotation) * XMConvertToRadians(m_rotationSpeed * deltaTime))));
		XMStoreFloat3A(&eye, XMVector3TransformCoord(XMLoadFloat3A(&eye), XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3A(&m_cameraRotation) * XMConvertToRadians(m_rotationSpeed * deltaTime))));
		XMStoreFloat3A(&eye, XMVector3TransformCoord(XMLoadFloat3A(&eye), XMMatrixTranslationFromVector(XMLoadFloat3A(&m_position))));
		m_camera.SetLookAt(eye, m_position, m_up);
	}

	m_camera.Update(deltaTime);

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

	for (CBulletObject& bullet : m_bullet)
	{
		if (!bullet.GetActive())
		{
			continue;
		}

		bullet.Update(deltaTime);
	}
}

void CTankPlayer::Render(HDC hDCFrameBuffer)
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
