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

	void SetTarget(CGameObject* target);

	virtual void AddRotationAngle(const float pitch, const float yaw, const float roll) override;
	virtual void AddRotationAngle(const XMFLOAT3A& rotate) override;
	void FireBullet();

	virtual void Rotate(const float deltaTime) override;
	virtual void Move(const float deltaTime) override;
	virtual void Collide(const float deltaTime) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(HDC hDCFrameBuffer) override;

private:
	CGameObject* m_target;

	CGameObject* m_turret;
	CGameObject* m_gun;

	float m_targetDistance;

	float m_searchTime;

	std::vector<CBulletObject> m_bullet;
	float m_coolTime;
};


