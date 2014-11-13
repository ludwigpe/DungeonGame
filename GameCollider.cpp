#include "GameCollider.h"
#include "Renderer.h"

GameCollider::GameCollider()
{
	m_boundingBox = NULL;
}

GameCollider::~GameCollider()
{
	if(m_boundingBox)
	{
		delete m_boundingBox;
		m_boundingBox = NULL;
	}
		
}
void GameCollider::DrawBounds(Renderer* renderer)
{

	const unsigned short index[] = {2, 3, 0,	// front
									0, 3, 1,
									6, 2, 4,	// left
									4, 0, 2,
									7, 6, 5,	// back
									5, 6, 4,
									3, 7, 1,	// right
									1, 7, 5,
									5, 1, 4,	// bottom
									4, 1, 0,
									2, 6, 3,	// top
									3, 6, 7};

	std::vector<D3DXVECTOR3>* bounds = m_boundingBox->GetBoundsRef();	
	D3DXVECTOR3 worldBounds[8];
	for( int i = 0; i < 8; i++ )
	{
		D3DXVECTOR3 point = bounds->at(i);
		worldBounds[i] = point;
	}
	
	renderer->GetDevice()->SetFVF(D3DFVF_XYZ);
	//renderer->GetDevice()->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, 36, 12, (void *)index, D3DFMT_INDEX16, bounds, sizeof(D3DXVECTOR3));
	renderer->GetDevice()->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, 36, 12, (void *)index, D3DFMT_INDEX16, worldBounds, sizeof(D3DXVECTOR3));
	
}