#include "Floor.h"

Floor::Floor(Mesh* mesh) : GameObject(mesh)
{
	//m_bottomLeft = D3DXVECTOR3(0,0,0);
	//m_topRight = D3DXVECTOR3(0,0,0);
	//ComputeBoundingBox();
	m_width  = -1.0f;
	m_height = -1.0f;
	m_depth  = -1.0f;
	m_computedBoundingBox = false;
	m_tag = "Floor";
}

bool Floor::ComputeBoundingBox()
{
	if( m_computedBoundingBox) return true;
	BYTE* pVertices = NULL;
	LPD3DXMESH mesh = m_mesh->GetD3DMesh();
	HRESULT hr = mesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVertices);
	if(FAILED(hr))
		return false;
	D3DXComputeBoundingBox((D3DXVECTOR3*)pVertices, mesh->GetNumVertices(), mesh->GetNumBytesPerVertex(), &m_bottomLeft, &m_topRight);
	mesh->UnlockVertexBuffer();
	m_computedBoundingBox = true;
	return true;
}

float Floor::GetWidth()
{
	if(m_width < 0)
	{
		if(!ComputeBoundingBox())
		{
			return 0.0f;
		}
		m_width = abs(m_topRight.x - m_bottomLeft.x);
	}
	

	return m_width;
}

float Floor::GetHeight()
{
	if(m_height < 0)
	{
		if(!ComputeBoundingBox())
		{
			return 0.0f;
		}
		m_height = abs(m_topRight.y - m_bottomLeft.y);
	}
	

	return m_height;
}

float Floor::GetDepth()
{
	if(m_depth < 0)
	{
		if(!ComputeBoundingBox())
		{
			return 0.0f;
		}
		m_depth = abs(m_topRight.z - m_bottomLeft.z);
	}

	return m_depth;
}

