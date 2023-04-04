#include "stdafx.h"
#include "GraphicsPipeline.h"

XMFLOAT4X4A CGraphicsPipeline::m_worldMatrix{};
XMFLOAT4X4A CGraphicsPipeline::m_cameraProjectMatrix{};
CViewport CGraphicsPipeline::m_viewport{};

XMFLOAT3A CGraphicsPipeline::worldTransform(const XMFLOAT3A& model)
{
	XMFLOAT3A result = model;
	XMStoreFloat3A(&result, XMVector3TransformCoord(XMLoadFloat3A(&result), XMLoadFloat4x4A(&m_worldMatrix)));
	return result;
}

XMFLOAT3A CGraphicsPipeline::cameraProejctTransform(const XMFLOAT3A& world)
{
	XMFLOAT3A result = world;
	XMStoreFloat3A(&result, XMVector3TransformCoord(XMLoadFloat3A(&result), XMLoadFloat4x4A(&m_cameraProjectMatrix)));
	return result;
}

//XMFLOAT3A CGraphicsPipeline::cameraTransform(const XMFLOAT3A& world)
//{
//	XMFLOAT3A result;
////	XMStoreFloat3A(&result, XMVector3TransformCoord(XMLoadFloat3A(&world), XMLoadFloat4x4A(&m_selectedCamera.GetCameraMatrix())));
//	return result;
//}
//
//XMFLOAT3A CGraphicsPipeline::perspectiveProjectTransform(const XMFLOAT3A& camera)
//{
//	XMFLOAT3A result;
////	XMStoreFloat3A(&result, XMVector3TransformCoord(XMLoadFloat3A(&camera), XMLoadFloat4x4A(&m_selectedCamera.GetPerspectiveProjectMatrix())));
//	return result;
//}


XMFLOAT3A CGraphicsPipeline::viewportTransform(const XMFLOAT3A& project)
{
	XMFLOAT3A result = project;
	result.x = m_viewport.GetLeft() + project.x * FRAMEBUFFER_WIDTH / 2 + FRAMEBUFFER_WIDTH / 2;
	result.y = m_viewport.GetTop() - project.y * FRAMEBUFFER_HEIGHT / 2 + FRAMEBUFFER_HEIGHT / 2;

	return result;
}

XMFLOAT3A CGraphicsPipeline::Project(const XMFLOAT3A& model)
{
	return cameraProejctTransform(worldTransform(model));
}

void CGraphicsPipeline::SetWorldMatrix(const XMFLOAT4X4A& worldMatrix)
{
	m_worldMatrix = worldMatrix;
}

void CGraphicsPipeline::SetCameraProejectdMatrix(const XMFLOAT4X4A& cameraProjectMatrix)
{
	m_cameraProjectMatrix = cameraProjectMatrix;
}

void CGraphicsPipeline::SetViewport(const CViewport& viewport)
{
	m_viewport = viewport;
}
