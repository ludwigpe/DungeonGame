#include "BoundingBox.h"
#include "Mesh.h"
BoundingBox::BoundingBox(D3DXVECTOR3 minBound, D3DXVECTOR3 maxBound)
{
	/*if (m_minBound == m_maxBound)
		delete this;
		*/
	m_minBound = minBound;
	m_maxBound = maxBound;

	SetupBoundingBox();
}
BoundingBox::BoundingBox(Mesh* mesh)
{
	if(mesh == NULL)
		return;

	BYTE* pVertices = NULL;
	LPD3DXMESH d3d_mesh = mesh->GetD3DMesh();
	HRESULT hr = d3d_mesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVertices);
	if(FAILED(hr))
		return;
	D3DXComputeBoundingBox((D3DXVECTOR3*)pVertices, d3d_mesh->GetNumVertices(), d3d_mesh->GetNumBytesPerVertex(), &m_minBound, &m_maxBound);
	d3d_mesh->UnlockVertexBuffer();

	SetupBoundingBox();
}

void BoundingBox::SetupBoundingBox()
{
	if(m_minBound == m_maxBound)
		return;

	m_width	 =	abs(m_maxBound.x - m_minBound.x);
	m_height =	abs(m_maxBound.y - m_minBound.y);
	m_depth  =	abs(m_maxBound.z - m_minBound.z);

	m_bounds.clear();
	m_bounds.resize(8);

	m_bounds[0] = D3DXVECTOR3( m_minBound.x, m_minBound.y, m_minBound.z ); // xyz
	m_bounds[1] = D3DXVECTOR3( m_maxBound.x, m_minBound.y, m_minBound.z ); // Xyz
	m_bounds[2] = D3DXVECTOR3( m_minBound.x, m_maxBound.y, m_minBound.z ); // xYz
	m_bounds[3] = D3DXVECTOR3( m_maxBound.x, m_maxBound.y, m_minBound.z ); // XYz
	m_bounds[4] = D3DXVECTOR3( m_minBound.x, m_minBound.y, m_maxBound.z ); // xyZ
	m_bounds[5] = D3DXVECTOR3( m_maxBound.x, m_minBound.y, m_maxBound.z ); // XyZ
	m_bounds[6] = D3DXVECTOR3( m_minBound.x, m_maxBound.y, m_maxBound.z ); // xYZ
	m_bounds[7] = D3DXVECTOR3( m_maxBound.x, m_maxBound.y, m_maxBound.z ); // XYZ
}
BoundingBox::~BoundingBox()
{
	m_bounds.clear();
	
}