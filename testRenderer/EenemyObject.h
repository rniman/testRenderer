#pragma once
#include "GameObject.h"
#include "BulletObject.h"


class CEenemyObject : public CGameObject
{
public:
	CEenemyObject();
	virtual ~CEenemyObject();

	UINT GetHP() const;

	virtual void AddRotationAngle(const float pitch, const float yaw, const float roll) override;
	virtual void AddRotationAngle(const XMFLOAT3A& rotate) override;

	virtual void Rotate(const float deltaTime) override;
	virtual void Move(const float deltaTime) override;
	virtual void Update(const float deltaTime) override;
	virtual void Collide(const float deltaTima) override;
	virtual void Render(HDC hDCFrameBuffer) override;

protected:
	UINT m_hp;
};

/// <CEenemyObject>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CEnemyTank>

class CEnemyTank : public CEenemyObject
{
public:
	CEnemyTank();
	virtual ~CEnemyTank();

	CGameObject* GetTurret() const;
	CGameObject* GetGun() const;
	std::vector<CBulletObject>& GetBullets();

	void SetMove(bool bMove);
	void SetTarget(CGameObject* target);

	virtual void AddRotationAngle(const float pitch, const float yaw, const float roll) override;
	virtual void AddRotationAngle(const XMFLOAT3A& rotate) override;
	void FireBullet();
	void FindTarget();
	void RotateToTarget(const float deltaTime);

	virtual void Rotate(const float deltaTime) override;
	virtual void Move(const float deltaTime) override;
	virtual void Collide(const float deltaTime) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(HDC hDCFrameBuffer) override;

private:
	bool m_bMove{ true };

	XMFLOAT3A m_look;

	XMFLOAT3A m_oldPosition;
	XMFLOAT3A m_oldTotalRotation{ 0.0f, 0.0f,0.0f };
	float m_elipsedRotation{ 0.0f };
	float m_remainRotation{ 0.0f };
	float m_remainTurretRotation{ 0.0f };
	float m_remainGunRotation{ 0.0f };

	float m_targetDistance;
	XMFLOAT3A m_moveDirection;
	float m_searchTime;

	CGameObject* m_target;
	bool m_bLockTarget{ false };

	CGameObject* m_turret;
	CGameObject* m_gun;

	std::vector<CBulletObject> m_bullet;
	float m_coolTime;
};


