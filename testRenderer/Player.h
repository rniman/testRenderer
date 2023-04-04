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

	void SetCamera(const CCamera& camera);
	void SetDirection();

	void HandleInput(DWORD direction);
	void Update(const float deltaTime);

private:
	float m_speed;

	CCamera m_camera;

	XMFLOAT3A m_moveDirection;
	XMFLOAT3A m_offSet;
};