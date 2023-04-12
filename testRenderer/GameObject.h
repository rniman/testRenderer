#pragma once
#include "Mesh.h"

class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();

	CGameObject(const CGameObject& other);
	CGameObject& operator=(const CGameObject& other);

	bool GetActive() const;
	bool GetPickingDetection() const;
	const std::unique_ptr<CGameObject>& GetParent() const;
	const std::unique_ptr<CGameObject>& GetChild() const;
	const std::unique_ptr<CGameObject>& GetSibling() const;

	void SetMesh(const std::shared_ptr<CMesh>& mesh);
	DWORD SetColor(const DWORD color);
	void SetPosition(const float x, const float y, const float z);
	void SetPosition(const XMFLOAT3A& position);
	void SetPickingDetection(const bool detection);
	void SetOOBB();

	void AddRotationAngle(const float pitch, const float yaw, const float roll);
	void AddRotationAngle(const XMFLOAT3A& rotate);
	
	bool CheckPicking(const XMFLOAT3A& ray, const XMFLOAT4X4A& cameraMatrix, float& distance);

	virtual void Rotate(const float deltaTime);
	virtual void Move(const float deltaTime);

	virtual void Update(const float deltaTime);
	virtual void Render(HDC hDCFrameBuffer);

protected:
	bool m_active;

	std::shared_ptr<CMesh> m_mesh;
	DWORD m_color;

	std::unique_ptr<CGameObject> m_parent;
	std::unique_ptr<CGameObject> m_child;
	std::unique_ptr<CGameObject> m_sibling;

	XMFLOAT4X4A m_worldMatrix;
	XMFLOAT3A m_position;
	XMFLOAT3A m_totalRotation;

	float m_rotationSpeed;
	float m_moveSpeed;

	bool m_pickingDetection;
	BoundingOrientedBox m_OOBB;
private:
};


