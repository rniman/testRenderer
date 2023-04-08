#pragma once
#include "GameObject.h"

class CBulletObject : public CGameObject
{
public:
	CBulletObject();
	virtual ~CBulletObject();

	void SetActive();
	void SetForward(const XMFLOAT3A& forward);
	
	virtual void Rotate(const float deltaTime) override;
	virtual void Move(const float deltaTime) override;

	virtual void Update(const float deltaTime) override;
	virtual void Render(HDC hDCFrameBuffer) override;

private:
	XMFLOAT3A m_forward;

	float m_elipsed;
	float m_duration;
};

