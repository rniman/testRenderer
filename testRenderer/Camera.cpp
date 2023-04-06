#include "stdafx.h"
#include "Camera.h"

CViewport::CViewport(const unsigned left, const unsigned top, const unsigned width, const unsigned height)
	: m_left{ left }
	, m_top{ top }
	, m_width{ width }
	, m_height{ height }
{
}

CViewport::~CViewport()
{
}

unsigned CViewport::GetLeft() const
{
	return m_left;
}

unsigned CViewport::GetTop() const
{
	return m_top;
}

void CViewport::SetViewport(const unsigned left, const unsigned top, const unsigned width, const unsigned height)
{
	m_left = left;
	m_top = top;
	m_width = width;
	m_height = height;
}

/// <CViewport>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CCamera>

CCamera::CCamera()
	: m_position{0.0f, 0.0f, 0.0f}
	, m_right{1.0f, 0.0f, 0.0f}
	, m_look{0.0f, 0.0f, 1.0f}
	, m_up{0.0f, 1.0f, 0.0f}
	, m_FOVAngle{60.0f}
	, m_aspect{ (float)FRAMEBUFFER_WIDTH / (float)FRAMEBUFFER_HEIGHT}
{
	XMStoreFloat4x4A(&m_cameraMatrix, XMMatrixIdentity());
	XMStoreFloat4x4A(&m_perspectiveProjectMatrix, XMMatrixIdentity());
	XMStoreFloat4x4A(&m_cameraProjectMatrix, XMMatrixIdentity());
}

CCamera::~CCamera()
{
}

CViewport CCamera::GetViewport() const
{
	return m_viewport;
}

XMFLOAT3A CCamera::GetPosition() const
{
	return m_position;
}

const XMFLOAT3A& CCamera::GetRight() const
{
	return m_right;
}

const XMFLOAT3A& CCamera::GetUp() const
{
	return m_up;
}

const XMFLOAT3A& CCamera::GetLook() const
{
	return m_look;
}

XMFLOAT4X4A CCamera::GetCameraMatrix() const
{
	return m_cameraMatrix;
}

XMFLOAT4X4A CCamera::GetCameraProjectMatrix() const
{
	return m_cameraProjectMatrix;
}

XMFLOAT4X4A CCamera::GetPerspectiveProjectMatrix() const
{
	return m_perspectiveProjectMatrix;
}


void CCamera::SetPosition(const XMFLOAT3A& position)
{
	m_position = position;
}

void CCamera::SetLookAt(const XMFLOAT3A& eye, const XMFLOAT3A& look, const XMFLOAT3A& up)
{
	m_position = eye;

	XMStoreFloat4x4A(&m_cameraMatrix, XMMatrixLookAtLH(XMLoadFloat3A(&eye), XMLoadFloat3A(&look), XMLoadFloat3A(&up)));

	m_right = { m_cameraMatrix._11, m_cameraMatrix._21, m_cameraMatrix._31 };
	m_up = { m_cameraMatrix._12, m_cameraMatrix._22, m_cameraMatrix._32 };
	m_look = { m_cameraMatrix._13, m_cameraMatrix._23, m_cameraMatrix._33 };

	XMStoreFloat3A(&m_right, XMVector3Normalize(XMLoadFloat3A(&m_right)));
	XMStoreFloat3A(&m_up, XMVector3Normalize(XMLoadFloat3A(&m_up)));
	XMStoreFloat3A(&m_look, XMVector3Normalize(XMLoadFloat3A(&m_look)));
}

void CCamera::SetLookTo(const XMFLOAT3A& eye, const XMFLOAT3A& look, const XMFLOAT3A& up)
{
	m_position = eye;
	
	XMStoreFloat4x4A(&m_cameraMatrix, XMMatrixLookToLH(XMLoadFloat3A(&eye), XMLoadFloat3A(&look), XMLoadFloat3A(&up)));

	m_right = { m_cameraMatrix._11, m_cameraMatrix._21, m_cameraMatrix._31 };
	m_up = { m_cameraMatrix._12, m_cameraMatrix._22, m_cameraMatrix._32 };
	m_look = { m_cameraMatrix._13, m_cameraMatrix._23, m_cameraMatrix._33 };

	XMStoreFloat3A(&m_right, XMVector3Normalize(XMLoadFloat3A(&m_right)));
	XMStoreFloat3A(&m_up, XMVector3Normalize(XMLoadFloat3A(&m_up)));
	XMStoreFloat3A(&m_look, XMVector3Normalize(XMLoadFloat3A(&m_look)));
}

void CCamera::SetCameraMatrix()
{
	XMStoreFloat4x4A(&m_cameraMatrix, XMMatrixLookToLH(XMLoadFloat3A(&m_position), XMLoadFloat3A(&m_look), XMLoadFloat3A(&m_up)));

	m_right = { m_cameraMatrix._11, m_cameraMatrix._21, m_cameraMatrix._31 };
	m_up = { m_cameraMatrix._12, m_cameraMatrix._22, m_cameraMatrix._32 };
	m_look = { m_cameraMatrix._13, m_cameraMatrix._23, m_cameraMatrix._33 };

	XMStoreFloat3A(&m_right, XMVector3Normalize(XMLoadFloat3A(&m_right)));
	XMStoreFloat3A(&m_up, XMVector3Normalize(XMLoadFloat3A(&m_up)));
	XMStoreFloat3A(&m_look, XMVector3Normalize(XMLoadFloat3A(&m_look)));
}

void CCamera::SetProjectMatrix(const float nearPlane, const float farPlane)
{
	XMStoreFloat4x4A(&m_perspectiveProjectMatrix, XMMatrixPerspectiveFovLH( XMConvertToRadians(m_FOVAngle), m_aspect, nearPlane, farPlane));
}

void CCamera::SetCameraProjectMatrix()
{
	XMStoreFloat4x4A(&m_cameraProjectMatrix, XMMatrixMultiply(XMLoadFloat4x4A(&m_cameraMatrix), XMLoadFloat4x4A(&m_perspectiveProjectMatrix)));
}

void CCamera::SetFOVAngle(const float FOVAngle)
{
	m_FOVAngle = FOVAngle;
}

void CCamera::SetViewport(const CViewport& viewport)
{
	m_viewport = viewport;
}

void CCamera::Move(const XMFLOAT3A& shift)
{
	XMStoreFloat3A(&m_position, XMVectorAdd(XMLoadFloat3A(&m_position), XMLoadFloat3A(&shift)));
}

void CCamera::Rotate(const float pitch, const float yaw, const float roll)
{
	XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	
	XMStoreFloat3A(&m_right, XMVector3Normalize( XMVector3TransformNormal( XMLoadFloat3A(&m_right), rotateMatrix)));
	XMStoreFloat3A(&m_look, XMVector3Normalize( XMVector3TransformNormal( XMLoadFloat3A(&m_look), rotateMatrix)));
	XMStoreFloat3A(&m_up, XMVector3Normalize( XMVector3TransformNormal( XMLoadFloat3A(&m_up), rotateMatrix)));
}

void CCamera::Update(const float deltaTime)
{
	//행렬 설정
	//SetCameraMatrix();
	SetCameraProjectMatrix();
}

