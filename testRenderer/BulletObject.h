#pragma once
#include "GameObject.h"

class CBulletObject : public CGameObject
{
public:
	CBulletObject();
	virtual ~CBulletObject();

	UINT GetDamge() const;

	void SetActive();
	void SetForward(const XMFLOAT3A& forward);
	void SetDamage(const float hp);

	void DeleteBullet();

	virtual void Rotate(const float deltaTime) override;
	virtual void Move(const float deltaTime) override;

	virtual void Update(const float deltaTime) override;
	virtual void Render(HDC hDCFrameBuffer) override;

private:
	XMFLOAT3A m_forward;

	UINT m_damage;

	float m_elipsed;
	float m_duration;
};

