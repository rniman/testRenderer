#pragma once

class CViewport
{
public:
	CViewport(const unsigned left = 0, const unsigned top = 0, const unsigned width = FRAMEBUFFER_WIDTH, const unsigned height = FRAMEBUFFER_HEIGHT);
	~CViewport();

	unsigned GetLeft() const;
	unsigned GetTop() const;

	void SetViewport(const unsigned left = 0, const unsigned top = 0, const unsigned width = FRAMEBUFFER_WIDTH, const unsigned height = FRAMEBUFFER_HEIGHT);

private:
	unsigned m_left;
	unsigned m_top;
	unsigned m_width;
	unsigned m_height;

};

/// <CViewport>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CCamera>

class CCamera
{
public:
	CCamera();
	~CCamera();

	CViewport GetViewport() const;
	XMFLOAT3A GetPosition() const;
	//XMFLOAT3A GetRight() const;
	//XMFLOAT3A GetUp() const;
	//XMFLOAT3A GetLook() const;

	const XMFLOAT3A& GetRight() const;
	const XMFLOAT3A& GetUp() const;
	const XMFLOAT3A& GetLook() const;

	XMFLOAT4X4A GetCameraMatrix() const;
	XMFLOAT4X4A GetCameraProjectMatrix() const;
	XMFLOAT4X4A GetPerspectiveProjectMatrix() const;

	void SetPosition(const XMFLOAT3A& position);
	void SetLookAt(const XMFLOAT3A& eye, const XMFLOAT3A& look, const XMFLOAT3A& up);
	void SetLookTo(const XMFLOAT3A& eye, const XMFLOAT3A& look, const XMFLOAT3A& up = {0.0f, 1.0f, 0.0f});
	
	void SetCameraMatrix();

	void SetProjectMatrix(const float nearPlane, const float farPlane);
	void SetCameraProjectMatrix();

	void SetFOVAngle(const float FOVAngle);

	void SetViewport(const CViewport& viewport);

	void Move(const XMFLOAT3A& shift);
	void Rotate(const float pitch, const float yaw, const float roll);

	void Update(const float deltaTime);
	//void Rotate(const XMFLOAT3A& axies, const float angle);
private:
	XMFLOAT3A m_position;
	XMFLOAT3A m_right;
	XMFLOAT3A m_look;
	XMFLOAT3A m_up;

	XMFLOAT4X4A m_cameraMatrix;
	XMFLOAT4X4A m_perspectiveProjectMatrix;
	XMFLOAT4X4A m_cameraProjectMatrix;

	float m_FOVAngle;
	float m_aspect;

	CViewport m_viewport;
};

