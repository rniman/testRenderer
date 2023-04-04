#pragma once
#include "GraphicsPipeline.h"

class CVertex
{
public:
	CVertex();
	CVertex(const XMFLOAT3A& position);
	CVertex(const float x, const float y, const float z);
	~CVertex();
	
	XMFLOAT3A GetPosition() const;

private:
	XMFLOAT3A m_position;
};

/// <CVertex>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CPolygon>

class CPolygon
{
public:
	CPolygon();
	CPolygon(const int numVertices);
	~CPolygon();

	CPolygon(const CPolygon& other);
	CPolygon& operator=(const CPolygon& other);

	CPolygon(CPolygon&& polygon);
	CPolygon& operator=(CPolygon&& other);

	std::vector<CVertex> GetBuffer() const;

	void SetVertex(const int index, const CVertex& vertex);

private:
	std::vector<CVertex> m_verticesBuffer;
};

/// <CPolygon>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CMesh>

class CMesh
{
public:
	CMesh();
	CMesh(const int numPolygons);
	virtual ~CMesh();

	void SetPolygon(const int index, CPolygon& polygon);

	void Render(HDC hDCFrameBuffer);

protected:
	std::vector<std::unique_ptr<CPolygon>> m_polygonsBuffer;
};

/// <CMesh>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CCube>

class CCube : public CMesh
{
public:
	CCube(const float width = 4.0f, const float height = 4.0f, const float depth = 4.0f);
	virtual ~CCube() override;
};
