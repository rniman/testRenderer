#include "stdafx.h"
#include "Mesh.h"

CVertex::CVertex()
	: m_position{ XMFLOAT3A(0.0f, 0.0f, 0.0f)}
{
}

CVertex::CVertex(const XMFLOAT3A& position)
	: m_position{position}
{
}

CVertex::CVertex(const float x, const float y, const float z)
	: m_position{x, y, z}
{
}

CVertex::~CVertex()
{
}

XMFLOAT3A CVertex::GetPosition() const
{
	return m_position;
}

/// <CVertex>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CPolygon>

CPolygon::CPolygon()
{
}

CPolygon::CPolygon(const int numVertices)
{
	m_verticesBuffer.resize(numVertices);
}

CPolygon::~CPolygon()
{
}

CPolygon::CPolygon(const CPolygon& other)
{
	m_verticesBuffer = other.m_verticesBuffer;
}

CPolygon& CPolygon::operator=(const CPolygon& other)
{
	m_verticesBuffer = other.m_verticesBuffer;
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

CPolygon::CPolygon(CPolygon&& other) noexcept
{
	m_verticesBuffer = other.m_verticesBuffer;

	other.m_verticesBuffer.clear();
}

CPolygon& CPolygon::operator=(CPolygon&& other) noexcept
{
	m_verticesBuffer = other.m_verticesBuffer;

	other.m_verticesBuffer.clear();
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

std::vector<CVertex> CPolygon::GetBuffer() const
{
	return m_verticesBuffer;
}

void CPolygon::SetVertex(const int index, const CVertex& vertex)
{
	if (index >= 0 && index < m_verticesBuffer.size())
	{
		m_verticesBuffer[index] = vertex;
	}
}

/// <CPolygon>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CMesh>

CMesh::CMesh()
	: m_OOBB{ BoundingOrientedBox() }
{
}

CMesh::CMesh(const int numPolygons)
	: m_OOBB{ BoundingOrientedBox() }
{
	m_polygonsBuffer.resize(numPolygons);
	//for (auto& pPolygon : m_polygonsBuffer)
	//	pPolygon = nullptr;
}

CMesh::~CMesh()
{
}

BoundingOrientedBox CMesh::GetOOBB() const
{
	return m_OOBB;
}

void CMesh::SetPolygon(const int index, CPolygon& polygon)
{
	if (index >= 0 && index < m_polygonsBuffer.size())
	{
		m_polygonsBuffer[index] = std::make_unique<CPolygon>(std::move(polygon));
	}
}

void Draw2DLine(HDC hDCFrameBuffer, const XMFLOAT3A& f3PreviousProject, const XMFLOAT3A& f3CurrentProject)
{
	XMFLOAT3A f3Previous = CGraphicsPipeline::viewportTransform(f3PreviousProject);
	XMFLOAT3A f3Current = CGraphicsPipeline::viewportTransform(f3CurrentProject);
	MoveToEx(hDCFrameBuffer, (long)f3Previous.x, (long)f3Previous.y, NULL);
	LineTo(hDCFrameBuffer, (long)f3Current.x, (long)f3Current.y);
}

void CMesh::Render(HDC hDCFrameBuffer)
{
	XMFLOAT3A firstProject, prevProject;
	bool bFirstProject, bPrevProject;

	for (int i = 0; i < m_polygonsBuffer.size(); ++i)
	{
		CVertex firstVertex = m_polygonsBuffer[i]->GetBuffer()[0];
		
		firstProject = prevProject = CGraphicsPipeline::Project(firstVertex.GetPosition());

		bFirstProject = bPrevProject = (firstProject.x <= 1) && (firstProject.x >= -1) && (firstProject.y <= 1) && (firstProject.y >= -1) && (firstProject.z >= 0) && (firstProject.z <= 1);

		for (int j = 1; j < m_polygonsBuffer[i]->GetBuffer().size(); ++j)
		{
			CVertex curVertex = m_polygonsBuffer[i]->GetBuffer()[j];
			XMFLOAT3A curProject = CGraphicsPipeline::Project(curVertex.GetPosition());
			bool bCurProject;

			bCurProject = (curProject.x <= 1) && (curProject.x >= -1) && (curProject.y <= 1) && (curProject.y >= -1) && (curProject.z >= 0) && (curProject.z <= 1);

			if(bPrevProject || bCurProject)
			{
				//그린다.
				Draw2DLine(hDCFrameBuffer, prevProject, curProject);
			}

			prevProject = curProject;
			bPrevProject = bCurProject;
		}

		if (bFirstProject || bPrevProject)
		{
			Draw2DLine(hDCFrameBuffer, prevProject, firstProject);
		}
	}
}

/// <CMesh>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CFloor>

CFloor::CFloor(const float width, const float height, const float depth, const unsigned divideNum)
	:CMesh(divideNum * divideNum)
{
	float smallWidth = width / divideNum;
	float smallDepth = depth / divideNum;

	unsigned index = 0;
	for (unsigned i = 0; i < divideNum; ++i)
	{
		for (unsigned j = 0; j < divideNum; ++j)
		{
			std::unique_ptr<CPolygon> Plane = std::make_unique<CPolygon>(4);
			Plane->SetVertex(0, CVertex(-width / 2 + smallWidth * (i + 1), 0.0f, depth / 2 - smallDepth * j));
			Plane->SetVertex(1, CVertex(-width / 2 + smallWidth * (i + 1), 0.0f, depth / 2 - smallDepth * (j + 1)));
			Plane->SetVertex(2, CVertex(-width / 2 + smallWidth * i, 0.0f, depth / 2 - smallDepth * (j + 1)));
			Plane->SetVertex(3, CVertex(-width / 2 + smallWidth * i, 0.0f, depth / 2 - smallDepth * j));

			SetPolygon(index++, *Plane.release());
		}
	}

	m_OOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(width / 2, height / 2, depth / 2), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

CFloor::~CFloor()
{
}

void CFloor::Render(HDC hDCFrameBuffer)
{
	XMFLOAT3A firstProject, prevProject, pPrevProject;
	bool bFirstProject, bPrevProject, bPPrevProject;

	for (int i = 0; i < m_polygonsBuffer.size(); ++i)
	{
		CVertex firstVertex = m_polygonsBuffer[i]->GetBuffer()[0];

		firstProject = prevProject = CGraphicsPipeline::Project(firstVertex.GetPosition());

		bFirstProject = bPrevProject = (firstProject.x <= 1) && (firstProject.x >= -1) && (firstProject.y <= 1) && (firstProject.y >= -1) && (firstProject.z >= 0) && (firstProject.z <= 1);

		for (int j = 1; j < m_polygonsBuffer[i]->GetBuffer().size(); ++j)
		{
			CVertex curVertex = m_polygonsBuffer[i]->GetBuffer()[j];
			XMFLOAT3A curProject = CGraphicsPipeline::Project(curVertex.GetPosition());
			bool bCurProject;

			bCurProject = (curProject.x <= 1) && (curProject.x >= -1) && (curProject.y <= 1) && (curProject.y >= -1) && (curProject.z >= 0) && (curProject.z <= 1);

			if (bPrevProject || bCurProject)
			{
				//그린다.
				Draw2DLine(hDCFrameBuffer, prevProject, curProject);
			}

			pPrevProject = prevProject;
			prevProject = curProject;

			bPPrevProject = bPrevProject;
			bPrevProject = bCurProject;
		}

		//if (bFirstProject || bPrevProject)
		//{
		//	Draw2DLine(hDCFrameBuffer, prevProject, firstProject);
		//	if (bFirstProject || bPPrevProject)
		//	{
		//		Draw2DLine(hDCFrameBuffer, pPrevProject, firstProject);
		//	}
		//}
	}
}

/// <CFloor>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CCube>

CCube::CCube(const float width, const float height, const float depth)
	: CMesh(6)
{
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
	float halfDepth = depth / 2.0f;

	std::unique_ptr<CPolygon> frontPlane = std::make_unique<CPolygon>(4);
	frontPlane->SetVertex(0, CVertex(halfWidth, halfHeight, halfDepth));
	frontPlane->SetVertex(1, CVertex(halfWidth, -halfHeight, halfDepth));
	frontPlane->SetVertex(2, CVertex(-halfWidth, -halfHeight, halfDepth));
	frontPlane->SetVertex(3, CVertex(-halfWidth, halfHeight, halfDepth));
	SetPolygon(0, *frontPlane.release());

	std::unique_ptr<CPolygon> leftPlane = std::make_unique<CPolygon>(4);
	leftPlane->SetVertex(0, CVertex(-halfWidth, halfHeight, -halfDepth));
	leftPlane->SetVertex(1, CVertex(-halfWidth, -halfHeight, -halfDepth));
	leftPlane->SetVertex(2, CVertex(-halfWidth, -halfHeight, halfDepth));
	leftPlane->SetVertex(3, CVertex(-halfWidth, halfHeight, halfDepth));
	SetPolygon(1, *leftPlane.release());

	std::unique_ptr<CPolygon> backPlane = std::make_unique<CPolygon>(4);
	backPlane->SetVertex(0, CVertex(halfWidth, halfHeight, -halfDepth));
	backPlane->SetVertex(1, CVertex(halfWidth, -halfHeight, -halfDepth));
	backPlane->SetVertex(2, CVertex(-halfWidth, -halfHeight, -halfDepth));
	backPlane->SetVertex(3, CVertex(-halfWidth, halfHeight, -halfDepth));
	SetPolygon(2, *backPlane.release());

	std::unique_ptr<CPolygon> rightPlane = std::make_unique<CPolygon>(4);
	rightPlane->SetVertex(0, CVertex(halfWidth, halfHeight, halfDepth));
	rightPlane->SetVertex(1, CVertex(halfWidth, -halfHeight, halfDepth));
	rightPlane->SetVertex(2, CVertex(halfWidth, -halfHeight, -halfDepth));
	rightPlane->SetVertex(3, CVertex(halfWidth, halfHeight, -halfDepth));
	SetPolygon(3, *rightPlane.release());

	std::unique_ptr<CPolygon> topPlane = std::make_unique<CPolygon>(4);
	topPlane->SetVertex(0, CVertex(halfWidth, halfHeight, halfDepth));
	topPlane->SetVertex(1, CVertex(halfWidth, halfHeight, -halfDepth));
	topPlane->SetVertex(2, CVertex(-halfWidth, halfHeight, -halfDepth));
	topPlane->SetVertex(3, CVertex(-halfWidth, halfHeight, halfDepth));
	SetPolygon(4, *topPlane.release());

	std::unique_ptr<CPolygon> bottomPlane = std::make_unique<CPolygon>(4);
	bottomPlane->SetVertex(0, CVertex(halfWidth, -halfHeight, -halfDepth));
	bottomPlane->SetVertex(1, CVertex(halfWidth, -halfHeight, halfDepth));
	bottomPlane->SetVertex(2, CVertex(-halfWidth, -halfHeight, halfDepth));
	bottomPlane->SetVertex(3, CVertex(-halfWidth, -halfHeight, -halfDepth));
	SetPolygon(5, *bottomPlane.release());
	
	m_OOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(halfWidth, halfHeight, halfDepth), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

CCube::~CCube()
{
}

void CCube::Render(HDC hDCFrameBuffer)
{
	CMesh::Render(hDCFrameBuffer);
}

/// <CCube>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CStick>

CStick::CStick(const float width, const float height, const float depth)
	:CMesh(6)
{
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;

	std::unique_ptr<CPolygon> frontPlane = std::make_unique<CPolygon>(4);
	frontPlane->SetVertex(0, CVertex(halfWidth, halfHeight, depth));
	frontPlane->SetVertex(1, CVertex(halfWidth, -halfHeight, depth));
	frontPlane->SetVertex(2, CVertex(-halfWidth, -halfHeight, depth));
	frontPlane->SetVertex(3, CVertex(-halfWidth, halfHeight, depth));
	SetPolygon(0, *frontPlane.release());

	std::unique_ptr<CPolygon> leftPlane = std::make_unique<CPolygon>(4);
	leftPlane->SetVertex(0, CVertex(-halfWidth, halfHeight, 0.0f));
	leftPlane->SetVertex(1, CVertex(-halfWidth, -halfHeight, 0.0f));
	leftPlane->SetVertex(2, CVertex(-halfWidth, -halfHeight, depth));
	leftPlane->SetVertex(3, CVertex(-halfWidth, halfHeight, depth));
	SetPolygon(1, *leftPlane.release());

	std::unique_ptr<CPolygon> backPlane = std::make_unique<CPolygon>(4);
	backPlane->SetVertex(0, CVertex(halfWidth, halfHeight, 0.0f));
	backPlane->SetVertex(1, CVertex(halfWidth, -halfHeight, 0.0f));
	backPlane->SetVertex(2, CVertex(-halfWidth, -halfHeight, 0.0f));
	backPlane->SetVertex(3, CVertex(-halfWidth, halfHeight, 0.0f));
	SetPolygon(2, *backPlane.release());

	std::unique_ptr<CPolygon> rightPlane = std::make_unique<CPolygon>(4);
	rightPlane->SetVertex(0, CVertex(halfWidth, halfHeight, depth));
	rightPlane->SetVertex(1, CVertex(halfWidth, -halfHeight, depth));
	rightPlane->SetVertex(2, CVertex(halfWidth, -halfHeight, 0.0f));
	rightPlane->SetVertex(3, CVertex(halfWidth, halfHeight, 0.0f));
	SetPolygon(3, *rightPlane.release());

	std::unique_ptr<CPolygon> topPlane = std::make_unique<CPolygon>(4);
	topPlane->SetVertex(0, CVertex(halfWidth, halfHeight, depth));
	topPlane->SetVertex(1, CVertex(halfWidth, halfHeight, 0.0f));
	topPlane->SetVertex(2, CVertex(-halfWidth, halfHeight, 0.0f));
	topPlane->SetVertex(3, CVertex(-halfWidth, halfHeight, depth));
	SetPolygon(4, *topPlane.release());

	std::unique_ptr<CPolygon> bottomPlane = std::make_unique<CPolygon>(4);
	bottomPlane->SetVertex(0, CVertex(halfWidth, -halfHeight, 0.0f));
	bottomPlane->SetVertex(1, CVertex(halfWidth, -halfHeight, depth));
	bottomPlane->SetVertex(2, CVertex(-halfWidth, -halfHeight, depth));
	bottomPlane->SetVertex(3, CVertex(-halfWidth, -halfHeight, 0.0f));
	SetPolygon(5, *bottomPlane.release());

	m_OOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, depth / 2.0f), XMFLOAT3(halfWidth, halfHeight, depth / 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

CStick::~CStick()
{
}

void CStick::Render(HDC hDCFrameBuffer)
{
	CMesh::Render(hDCFrameBuffer);
}

/// <CStick>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CTankMesh>

CTankMesh::CTankMesh()
	:CMesh(6)
{
	float halfWidth = 4.0f;
	float halfHeight = 1.0f;
	float halfDepth = 5.0f;

	std::unique_ptr<CPolygon> frontPlane = std::make_unique<CPolygon>(4);
	frontPlane->SetVertex(0, CVertex(halfWidth, halfHeight, halfDepth));
	frontPlane->SetVertex(1, CVertex(halfWidth, -halfHeight, halfDepth));
	frontPlane->SetVertex(2, CVertex(-halfWidth, -halfHeight, halfDepth));
	frontPlane->SetVertex(3, CVertex(-halfWidth, halfHeight, halfDepth));
	SetPolygon(0, *frontPlane.release());

	std::unique_ptr<CPolygon> leftPlane = std::make_unique<CPolygon>(4);
	leftPlane->SetVertex(0, CVertex(-halfWidth, halfHeight, -halfDepth));
	leftPlane->SetVertex(1, CVertex(-halfWidth, -halfHeight, -halfDepth));
	leftPlane->SetVertex(2, CVertex(-halfWidth, -halfHeight, halfDepth));
	leftPlane->SetVertex(3, CVertex(-halfWidth, halfHeight, halfDepth));
	SetPolygon(1, *leftPlane.release());

	std::unique_ptr<CPolygon> backPlane = std::make_unique<CPolygon>(4);
	backPlane->SetVertex(0, CVertex(halfWidth, halfHeight, -halfDepth));
	backPlane->SetVertex(1, CVertex(halfWidth, -halfHeight, -halfDepth));
	backPlane->SetVertex(2, CVertex(-halfWidth, -halfHeight, -halfDepth));
	backPlane->SetVertex(3, CVertex(-halfWidth, halfHeight, -halfDepth));
	SetPolygon(2, *backPlane.release());

	std::unique_ptr<CPolygon> rightPlane = std::make_unique<CPolygon>(4);
	rightPlane->SetVertex(0, CVertex(halfWidth, halfHeight, halfDepth));
	rightPlane->SetVertex(1, CVertex(halfWidth, -halfHeight, halfDepth));
	rightPlane->SetVertex(2, CVertex(halfWidth, -halfHeight, -halfDepth));
	rightPlane->SetVertex(3, CVertex(halfWidth, halfHeight, -halfDepth));
	SetPolygon(3, *rightPlane.release());

	std::unique_ptr<CPolygon> topPlane = std::make_unique<CPolygon>(4);
	topPlane->SetVertex(0, CVertex(halfWidth, halfHeight, halfDepth));
	topPlane->SetVertex(1, CVertex(halfWidth, halfHeight, -halfDepth));
	topPlane->SetVertex(2, CVertex(-halfWidth, halfHeight, -halfDepth));
	topPlane->SetVertex(3, CVertex(-halfWidth, halfHeight, halfDepth));
	SetPolygon(4, *topPlane.release());

	std::unique_ptr<CPolygon> bottomPlane = std::make_unique<CPolygon>(4);
	bottomPlane->SetVertex(0, CVertex(halfWidth, -halfHeight, -halfDepth));
	bottomPlane->SetVertex(1, CVertex(halfWidth, -halfHeight, halfDepth));
	bottomPlane->SetVertex(2, CVertex(-halfWidth, -halfHeight, halfDepth));
	bottomPlane->SetVertex(3, CVertex(-halfWidth, -halfHeight, -halfDepth));
	SetPolygon(5, *bottomPlane.release());

	m_OOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(halfWidth, halfHeight, halfDepth), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

CTankMesh::~CTankMesh()
{
}

void CTankMesh::Render(HDC hDCFrameBuffer)
{
	CMesh::Render(hDCFrameBuffer);
}

