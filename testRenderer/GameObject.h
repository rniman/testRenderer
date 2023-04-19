#pragma once
#include "Mesh.h"

class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();

	CGameObject(CGameObject&& other) noexcept;
	CGameObject& operator=(CGameObject&& other) noexcept;

	CGameObject(const CGameObject& other) = delete;
	CGameObject& operator=(const CGameObject& other) = delete;

	bool GetActive() const;
	CMesh* GetMesh() const;
	CGameObject* GetParent() const;
	CGameObject* GetChild() const;
	CGameObject* GetSibling() const;
	XMFLOAT4X4A GetWorldMatrix()const;
	XMFLOAT3A GetPosition() const;
	XMFLOAT3A GetTotalRotation() const;
	float GetRotateSpeed() const;
	CGameObject* GetCollidedObject() const;
	bool GetPickingDetection() const;
	BoundingOrientedBox GetOOBB() const;

	void SetMesh(const std::shared_ptr<CMesh>& mesh);
	DWORD SetColor(const DWORD color);
	void SetParent(CGameObject& parent);
	void SetChild(CGameObject& child);
	void SetSibling(CGameObject& sibling);
	void SetPosition(const float x, const float y, const float z);
	void SetPosition(const XMFLOAT3A& position);
	void SetRotationSpeed(const float rotationSpeed);
	void SetMoveSpeed(const float moveSpeed);
	void SetCollidedObject(CGameObject* gameObject);
	void SetPickingDetection(const bool detection);
	void SetOOBB();

	virtual void AddRotationAngle(const float pitch, const float yaw, const float roll);
	virtual void AddRotationAngle(const XMFLOAT3A& rotate);
	
	static bool CheckPicking(const CGameObject* gameObject, const XMFLOAT3A& ray, const XMFLOAT4X4A& cameraMatrix, float& distance);

	virtual void Rotate(const float deltaTime);
	virtual void Move(const float deltaTime);
	virtual void Update(const float deltaTime);
	virtual void Collide(const float deltaTime);
	virtual void Render(HDC hDCFrameBuffer);

	void reset();

	static void SetAllColor(CGameObject* gameObject, DWORD color);

protected:
	bool m_active;

	std::shared_ptr<CMesh> m_mesh;
	DWORD m_color;

	CGameObject* m_parent;
	CGameObject* m_child;
	CGameObject* m_sibling;

	XMFLOAT4X4A m_worldMatrix;
	XMFLOAT3A m_position;
	XMFLOAT3A m_totalRotation;

	float m_rotationSpeed;
	float m_moveSpeed;

	CGameObject* m_collidedObject;
	bool m_pickingDetection;
	BoundingOrientedBox m_OOBB;
};


