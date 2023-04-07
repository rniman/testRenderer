#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer()
	: CGameObject()
	, m_position{ m_worldMatrix._41, m_worldMatrix._42, m_worldMatrix._43 }
	, m_right{ XMFLOAT3A(1.0f,0.0f, 0.0f) }
	, m_up{ XMFLOAT3A(0.0f,1.0f, 0.0f) }
	, m_look{ XMFLOAT3A(0.0f,0.0f, 1.0f) }
	, m_speed{ 5.0f }
	, m_rotationSpeed{ 5.0f }
	, m_camera()
	, m_moveDirection{ 0.0f, 0.0f, 0.0f }
{
}

CPlayer::CPlayer(const CCamera& camera)
	: CGameObject()
	, m_position{ m_worldMatrix._41, m_worldMatrix._42, m_worldMatrix._43 }
	, m_right{ XMFLOAT3A(1.0f,0.0f, 0.0f) }
	, m_up{ XMFLOAT3A(0.0f,1.0f, 0.0f) }
	, m_look{ XMFLOAT3A(0.0f,0.0f, 1.0f) }
	, m_speed{ 5.0f }
	, m_rotationSpeed{ 5.0f }
	, m_camera(camera)
	, m_moveDirection{ 0.0f, 0.0f, 0.0f }
{
}

CPlayer::~CPlayer()
{
}

CCamera CPlayer::GetCamera() const
{
	return m_camera;
}

void CPlayer::SetPosition(const XMFLOAT3A& position)
{
	m_position = position;
}

void CPlayer::SetCamera(const CCamera& camera)
{
	m_camera = camera;
}

void CPlayer::SetDirection()
{
	m_moveDirection = XMFLOAT3A();
}

void CPlayer::SetRotationAngle(const float pitch, const float yaw, const float roll)
{
	m_pitch += pitch;
	m_yaw += yaw;
	m_roll += roll;
}

void CPlayer::Rotate(const float deltaTime)
{
	//회전 행렬 구하고
	//XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_pitch * m_rotationSpeed * deltaTime),	XMConvertToRadians(m_yaw * m_rotationSpeed * deltaTime),XMConvertToRadians(m_roll * m_rotationSpeed * deltaTime));

	//XMStoreFloat4x4A(&m_worldMatrix, rotateMatrix * XMLoadFloat4x4A(&m_worldMatrix));

	////기존 look에서 회전을 추가시킴
	//XMVECTOR look = XMVectorSet(m_look.x, m_look.y, m_look.z, 0.0f);
	//look = XMVector3TransformNormal(look, rotateMatrix);
	//XMStoreFloat3A(&m_look, look);

	//XMVECTOR up = XMVectorSet(m_up.x, m_up.y, m_up.z, 0.0f);
	//up = XMVector3TransformNormal(up, rotateMatrix);
	//XMStoreFloat3A(&m_up, up);

	//XMVECTOR right = XMVectorSet(m_right.x, m_right.y, m_right.z, 0.0f);
	//right = XMVector3TransformNormal(right, rotateMatrix);
	//XMStoreFloat3A(&m_right, right);


	////쿼터니언 사용
	//XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(m_pitch * m_rotationSpeed * deltaTime), XMConvertToRadians(m_yaw * m_rotationSpeed * deltaTime), XMConvertToRadians(m_roll * m_rotationSpeed * deltaTime));

	//XMStoreFloat4x4A(&m_worldMatrix, XMMatrixRotationQuaternion(quaternion) * XMLoadFloat4x4A(&m_worldMatrix));

	//XMVECTOR look = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	//XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//XMVECTOR right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	//XMStoreFloat3A(&m_look, XMVector3TransformNormal(look, XMLoadFloat4x4A(&m_worldMatrix)));
	//XMStoreFloat3A(&m_up, XMVector3TransformNormal(up, XMLoadFloat4x4A(&m_worldMatrix)));
	//XMStoreFloat3A(&m_right, XMVector3TransformNormal(right, XMLoadFloat4x4A(&m_worldMatrix)));

	//회전행렬 구하고
	XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_pitch * m_rotationSpeed * deltaTime), XMConvertToRadians(m_yaw * m_rotationSpeed * deltaTime), XMConvertToRadians(m_roll * m_rotationSpeed * deltaTime));

	XMStoreFloat4x4A(&m_worldMatrix, rotateMatrix * XMMatrixTranslationFromVector(XMLoadFloat3A(&m_position)));

	//월드 z축에 최종 회전을 추가
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
	XMStoreFloat4x4A(&m_worldMatrix, XMLoadFloat4x4A(&m_worldMatrix) * XMMatrixTranslationFromVector(XMLoadFloat3A(&m_moveDirection) * m_speed * deltaTime));

	m_position.x = m_worldMatrix._41;
	m_position.y = m_worldMatrix._42;
	m_position.z = m_worldMatrix._43;
}

void CPlayer::HandleInput(DWORD direction)
{
	if (direction)
	{
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
	//행렬을 업데이트하고 움직인다//
	Rotate(deltaTime);
	Move(deltaTime);

	m_camera.SetLookTo(m_position, m_look);
	m_camera.Update(deltaTime);
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
{
	m_camera.SetPosition(m_cameraOffset);
}

CTankPlayer::CTankPlayer(const CCamera& camera)
	: CPlayer(camera)
	, m_cameraOffset{ 0.0f, 10.0f, -30.0f }
{
	m_camera.SetPosition(m_cameraOffset);
}

CTankPlayer::~CTankPlayer()
{
}

void CTankPlayer::Update(const float deltaTime)
{
	Rotate(deltaTime);
	Move(deltaTime);

	m_position.x = m_worldMatrix._41;
	m_position.y = m_worldMatrix._42;
	m_position.z = m_worldMatrix._43;

	XMFLOAT3A eye;
	XMStoreFloat3A(&eye, XMVector3TransformCoord( XMLoadFloat3A(& m_cameraOffset), XMLoadFloat4x4A(&m_worldMatrix)));

	m_camera.SetLookTo(eye, m_look, m_up);
	m_camera.Update(deltaTime);
}

void CTankPlayer::Render(HDC hDCFrameBuffer)
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
