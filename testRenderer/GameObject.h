#pragma once
#include "Mesh.h"

class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();

	bool GetActive() const;

	void SetPosition(const float x, const float y, const float z);
	void SetPosition(const XMFLOAT3A& position);
	void SetMesh(const std::shared_ptr<CMesh>& mesh);
	void SetColor(const DWORD color);

	void AddRotationAngle(const float pitch, const float yaw, const float roll);
	void AddRotationAngle(const XMFLOAT3A& rotate);

	virtual void Rotate(const float deltaTime);
	virtual void Move(const float deltaTime);

	virtual void Update(const float deltaTime);
	virtual void Render(HDC hDCFrameBuffer);

protected:
	bool m_active;
	std::shared_ptr<CMesh> m_mesh;
	DWORD m_color;

	XMFLOAT4X4A m_worldMatrix;
	XMFLOAT3A m_position;
	XMFLOAT3A m_totalRotation;

	float m_rotationSpeed;
	float m_moveSpeed;
private:
};


