#pragma once
#include "BulletObject.h"
#include "Camera.h"

class CPlayer : public CGameObject
{
public:
	CPlayer();
	CPlayer(const CCamera& camera);
	virtual ~CPlayer();
	
	CCamera GetCamera() const;
	
	void SetCamera(const CCamera& camera);
	void SetDirection();
	void SetInctiveMoveForce();

	void Rotate(const float deltaTime) override;
	void Move(const float deltaTime) override;

	void HandleInput(DWORD direction);
	void Update(const float deltaTime) override;
	void Render(HDC hDCFrameBuffer) const override;

protected:
	XMFLOAT3A m_right;
	XMFLOAT3A m_up;
	XMFLOAT3A m_look;
	
	CCamera m_camera;

	bool m_bMoveForce;
	XMFLOAT3A m_moveDirection;

	float m_friction;
	float m_acceleration;
};

/// <CPlayer>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CTankPlayer>

constexpr UCHAR MAX_BULLET{ 50 };

class CTankPlayer : public CPlayer
{
public:
	CTankPlayer();
	CTankPlayer(const CCamera& camera);
	virtual ~CTankPlayer();

	void FireBullet();

	void Update(const float deltaTime) override;
	void Render(HDC hDCFrameBuffer) const override;
private:
	XMFLOAT3A m_cameraOffset;
	CCamera m_subCamera;
	bool m_bMainCamera;

	std::vector<CBulletObject> m_bullet;
	float m_coolTime;
};