#pragma once
#include "BulletObject.h"
#include "Camera.h"

class CPlayer : public CGameObject
{
public:
	CPlayer();
	CPlayer(const CCamera& camera);
	virtual ~CPlayer() override;
	
	CCamera GetCamera() const;
	
	void SetCamera(const CCamera& camera);
	void SetDirection();
	void SetInactiveMoveForce();

	virtual void AddRotationAngle(const float pitch, const float yaw, const float roll) override;
	virtual void AddCameraRotation(const float pitch, const float yaw, const float roll);

	void Rotate(const float deltaTime) override;
	void Move(const float deltaTime) override;

	virtual void HandleInput(DWORD direction);
	virtual void Update(const float deltaTime) override;
	virtual void Render(HDC hDCFrameBuffer) override;

protected:
	XMFLOAT3A m_right;
	XMFLOAT3A m_up;
	XMFLOAT3A m_look;
	
	CCamera m_camera;
	XMFLOAT3A m_cameraRotation;

	bool m_bMoveForce;
	XMFLOAT3A m_moveDirection;

	float m_friction;
	float m_acceleration;
};

/// <CPlayer>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CTankPlayer>

class CTankPlayer : public CPlayer
{
public:
	CTankPlayer();
	CTankPlayer(const CCamera& camera);
	~CTankPlayer() override;

	virtual void AddRotationAngle(const float pitch, const float yaw, const float roll) override;
	virtual void AddCameraRotation(const float pitch, const float yaw, const float roll) override;
	void FireBullet();

	void HandleInput(DWORD direction) override;
	void Update(const float deltaTime) override;
	void Render(HDC hDCFrameBuffer) override;
private:
	XMFLOAT3A m_cameraOffset;
	bool m_bMainCamera;
	float m_remainingRotation;
	
	XMFLOAT3A m_oldTotalRotation{ 0.0f, 0.0f,0.0f };
	float m_rotationNum = 0;

	CGameObject* m_turret;
	CGameObject* m_gun;

	std::vector<CBulletObject> m_bullet;
	float m_coolTime;
};