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

	BoundingOrientedBox GetOOBB() const;

	void SetPolygon(const int index, CPolygon& polygon);

	virtual void Render(HDC hDCFrameBuffer);

protected:
	BoundingOrientedBox m_OOBB;	//모델 좌표계의 바운딩 박스
	std::vector<std::unique_ptr<CPolygon>> m_polygonsBuffer;
};

/// <CMesh>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CFloor>

class CFloor : public CMesh
{
public:
	CFloor(const float width = 100.0f, const float height = 0.0f, const float depth = 100.0f, const unsigned divideNum = 10);
	virtual ~CFloor() override;

	virtual void Render(HDC hDCFrameBuffer);
};

/// <CFloor>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CCube>

class CCube : public CMesh
{
public:
	CCube(const float width = 4.0f, const float height = 4.0f, const float depth = 4.0f);
	virtual ~CCube() override;

	virtual void Render(HDC hDCFrameBuffer) override;
};

/// <CCube>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
/// <CTankMesh>

class CTankMesh : public CMesh
{
public:
	CTankMesh();
	virtual ~CTankMesh();

	virtual void Render(HDC hDCFrameBuffer) override;
};
