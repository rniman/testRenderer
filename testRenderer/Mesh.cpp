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

CPolygon::CPolygon(CPolygon&& other)
{
	m_verticesBuffer = other.m_verticesBuffer;

	other.m_verticesBuffer.clear();
}

CPolygon& CPolygon::operator=(CPolygon&& other)
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
{
}

CMesh::CMesh(const int numPolygons)
{
	m_polygonsBuffer.resize(numPolygons);
	//for (auto& pPolygon : m_polygonsBuffer)
	//	pPolygon = nullptr;
}

CMesh::~CMesh()
{
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
/// <CCube>

CCube::CCube(const float width, const float height, const float depth)
	:CMesh(6)
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
}

CCube::~CCube()
{
}
