#pragma once
#include "Camera.h"

//변환을 수행하는 클래스 -> 쉐이더가 할일을 대신하는 장치
class CGraphicsPipeline
{
public:
	static XMFLOAT3A worldTransform(const XMFLOAT3A& model);
	static XMFLOAT3A cameraProejctTransform(const XMFLOAT3A& world);
	//static XMFLOAT3A cameraTransform(const XMFLOAT3A& world);
	//static XMFLOAT3A perspectiveProjectTransform(const XMFLOAT3A& camera);
	static XMFLOAT3A viewportTransform(const XMFLOAT3A& project);

	static XMFLOAT3A Project(const XMFLOAT3A& model);

	static void SetWorldMatrix(const XMFLOAT4X4A& worldMatrix);
	static void SetCameraProejectdMatrix(const XMFLOAT4X4A& cameraProjectMatrix);
	static void SetViewport(const CViewport& viewport);

private:
	static XMFLOAT4X4A m_worldMatrix;
	static XMFLOAT4X4A m_cameraProjectMatrix;
	static CViewport m_viewport;
};

