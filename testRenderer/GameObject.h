#pragma once
#include "Mesh.h"


class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();

	void SetPosition(const float x, const float y, const float z);
	void SetPosition(const XMFLOAT3A& position);
	void SetMesh(const std::shared_ptr<CMesh>& mesh);
	void SetColor(const DWORD color);

	void Move();
	void Rotate(const float pitch, const float yaw, const float roll);

	void Update(const float deltaTime);
	void Render(HDC hDCFrameBuffer);

protected:
	bool m_active;
	XMFLOAT4X4A m_worldMatrix;
	std::shared_ptr<CMesh> m_mesh;

private:
	DWORD m_color;
};

