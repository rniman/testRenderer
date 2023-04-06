#pragma once
#include "GameObject.h"
#include "Camera.h"

class CPlayer : public CGameObject
{
public:
	CPlayer();
	CPlayer(const CCamera& camera);
	virtual ~CPlayer();
	
	CCamera GetCamera() const;
	
	void SetPosition(const XMFLOAT3A& position);
	void SetCamera(const CCamera& camera);
	void SetDirection();
	void SetRotationAngle(const float pitch, const float yaw, const float roll);

	void Rotate(const float deltaTime);
	void Move(const float deltaTime);

	void HandleInput(DWORD direction);
	void Update(const float deltaTime) override;
	void Render(HDC hDCFrameBuffer) override;

protected:
	XMFLOAT3A m_position;
	XMFLOAT3A m_right;
	XMFLOAT3A m_up;
	XMFLOAT3A m_look;
	
	float m_pitch{ 0.0f };
	float m_yaw{ 0.0f };
	float m_roll{ 0.0f };

	float m_speed;
	float m_rotationSpeed;

	CCamera m_camera;

	XMFLOAT3A m_moveDirection;
};

/// <CPlayer>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CTankPlayer>

class CTankPlayer : public CPlayer
{
public:
	CTankPlayer();
	CTankPlayer(const CCamera& camera);
	virtual ~CTankPlayer();

	void Update(const float deltaTime) override;
	void Render(HDC hDCFrameBuffer) override;
private:
	XMFLOAT3A m_cameraOffset;
};