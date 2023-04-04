#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer()
	: CGameObject()
	, m_speed{ 5.0f }
	, m_camera()
	, m_moveDirection{ 0.0f, 0.0f, 0.0f }
	, m_offSet{ 0.0f ,0.0f, 0.0f }
{
}

CPlayer::CPlayer(const CCamera& camera)
	: CGameObject()
	, m_speed{ 5.0f }
	, m_camera(camera)
	, m_moveDirection{ 0.0f, 0.0f, 0.0f }
	, m_offSet{ 0.0f ,0.0f, 0.0f }
{
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

void CPlayer::HandleInput(DWORD direction)
{
	if (direction)
	{
		if (direction & DIR_FORWARD) XMStoreFloat3A(&m_moveDirection, XMLoadFloat3A(&m_moveDirection) + XMLoadFloat3A(&m_camera.GetLook()));

		XMStoreFloat3A(&m_moveDirection, XMVector3Normalize(XMLoadFloat3A(&m_moveDirection)));
	}
}

void CPlayer::Update(const float deltaTime)
{
	XMStoreFloat3A(&m_offSet, XMLoadFloat3A(&m_moveDirection) * m_speed * deltaTime);
	m_camera.Move(m_offSet);
	m_camera.Update(deltaTime);
}

