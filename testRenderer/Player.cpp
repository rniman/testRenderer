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
	, m_acceleration{ 25.0f }
	, m_friction{ 15.0f }
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
	, m_acceleration{ 25.0f }
	, m_friction{ 15.0f }
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
	CGameObject::AddRotationAngle(pitch, yaw, roll);
}

void CPlayer::AddCameraRotation(const float pitch, const float yaw, const float roll)
{
	m_cameraRotation.x += pitch;
	m_cameraRotation.y += yaw;
	m_cameraRotation.z += roll;
}

void CPlayer::Rotate(const float deltaTime)
{
	//ȸ�� ��� ���ϰ�
	//XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_pitch * m_rotationSpeed * deltaTime),	XMConvertToRadians(m_yaw * m_rotationSpeed * deltaTime),XMConvertToRadians(m_roll * m_rotationSpeed * deltaTime));

	//XMStoreFloat4x4A(&m_worldMatrix, rotateMatrix * XMLoadFloat4x4A(&m_worldMatrix));

	////���� look���� ȸ���� �߰���Ŵ
	//XMVECTOR look = XMVectorSet(m_look.x, m_look.y, m_look.z, 0.0f);
	//look = XMVector3TransformNormal(look, rotateMatrix);
	//XMStoreFloat3A(&m_look, look);

	//XMVECTOR up = XMVectorSet(m_up.x, m_up.y, m_up.z, 0.0f);
	//up = XMVector3TransformNormal(up, rotateMatrix);
	//XMStoreFloat3A(&m_up, up);

	//XMVECTOR right = XMVectorSet(m_right.x, m_right.y, m_right.z, 0.0f);
	//right = XMVector3TransformNormal(right, rotateMatrix);
	//XMStoreFloat3A(&m_right, right);


	////���ʹϾ� ���
	//XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(m_pitch * m_rotationSpeed * deltaTime), XMConvertToRadians(m_yaw * m_rotationSpeed * deltaTime), XMConvertToRadians(m_roll * m_rotationSpeed * deltaTime));

	//XMStoreFloat4x4A(&m_worldMatrix, XMMatrixRotationQuaternion(quaternion) * XMLoadFloat4x4A(&m_worldMatrix));

	//XMVECTOR look = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	//XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//XMVECTOR right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	//XMStoreFloat3A(&m_look, XMVector3TransformNormal(look, XMLoadFloat4x4A(&m_worldMatrix)));
	//XMStoreFloat3A(&m_up, XMVector3TransformNormal(up, XMLoadFloat4x4A(&m_worldMatrix)));
	//XMStoreFloat3A(&m_right, XMVector3TransformNormal(right, XMLoadFloat4x4A(&m_worldMatrix)));

	//ȸ����� ���ϰ�
	XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3A(&m_totalRotation) * XMConvertToRadians(m_rotationSpeed * deltaTime));

	XMStoreFloat4x4A(&m_worldMatrix, rotateMatrix * XMMatrixTranslationFromVector(XMLoadFloat3A(&m_position)));

	//���� z�࿡ ���� ȸ���� �߰�
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
		if (m_moveSpeed < 10.0f)
		{
			m_moveSpeed += deltaTime * (m_acceleration - m_friction);
			if (m_moveSpeed > 10.0f)
			{
				m_moveSpeed = 10.0f;
			}
		}
	}
	else
	{
		if (m_moveSpeed > 0.0f)
		{
			m_moveSpeed -= deltaTime * m_friction * 2;
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
		if (direction & DIR_UP) XMStoreFloat3A(&m_moveDirection, XMLoadFloat3A(&m_moveDirection) + XMLoadFloat3A(&m_up));
		if (direction & DIR_DOWN) XMStoreFloat3A(&m_moveDirection, XMLoadFloat3A(&m_moveDirection) - XMLoadFloat3A(&m_up));
		XMStoreFloat3A(&m_moveDirection, XMVector3Normalize(XMLoadFloat3A(&m_moveDirection)));
	}
}

void CPlayer::Update(const float deltaTime)
{
	//����� ������Ʈ�ϰ� �����δ�//
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
	, m_turret{ nullptr }
	, m_gun{ nullptr }
	, m_bullet(MAX_BULLET)
	, m_coolTime{ 0.0f }
{
	SetPosition(XMFLOAT3A(0.0f, 1.0f, 0.0f));
	m_camera.SetPosition(m_cameraOffset);

	m_child = new CGameObject();
	m_turret = m_child;
	m_turret->SetParent(*this);
	m_turret->SetRotationSpeed(m_rotationSpeed);

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
	, m_turret{ nullptr }
	, m_gun{ nullptr }
	, m_bullet(MAX_BULLET)
	, m_coolTime{ 0.0f }
{
	SetPosition(XMFLOAT3A(0.0f, 1.0f, 0.0f));
	m_camera.SetPosition(m_cameraOffset);
	
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

CTankPlayer::~CTankPlayer() 
{
	m_turret = nullptr;
	m_gun = nullptr;
}

void CTankPlayer::AddRotationAngle(const float pitch, const float yaw, const float roll)
{
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

		bullet.SetForward(m_look);
		m_coolTime = 2.0f;
		break;
	}
}

void CTankPlayer::Update(const float deltaTime) 
{
	Rotate(deltaTime);
	Move(deltaTime);
	SetOOBB();

	XMFLOAT3A eye;
	if (m_bMainCamera)
	{
		XMStoreFloat3A(&eye, XMVector3TransformCoord(XMLoadFloat3A(&m_cameraOffset), XMLoadFloat4x4A(&m_worldMatrix)));
		XMFLOAT3A look;
		XMStoreFloat3(&look, XMVector3Normalize(XMVectorSubtract(XMLoadFloat3A(&m_position), XMLoadFloat3(&eye))));
		m_camera.SetLookTo(eye, look, m_up);
	}
	else
	{
		XMStoreFloat3A(&eye, XMVector3TransformCoord(XMLoadFloat3A(&m_cameraOffset), XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3A(&m_cameraRotation) * XMConvertToRadians(m_rotationSpeed * deltaTime))));
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
