#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <d3dx9math.h>
#include <vector>
class Mesh;
class BoundingBox
{
private:
	D3DXVECTOR3 m_minBound;
	D3DXVECTOR3 m_maxBound;

	std::vector<D3DXVECTOR3> m_bounds;
	float m_width;
	float m_height;
	float m_depth;

public:
	BoundingBox(D3DXVECTOR3 minBound, D3DXVECTOR3 maxBound);
	BoundingBox(Mesh* mesh);
	~BoundingBox();
	void SetupBoundingBox();
	std::vector<D3DXVECTOR3>* GetBoundsRef() { return &m_bounds; }
	std::vector<D3DXVECTOR3> GetBounds() { return m_bounds; }
	D3DXVECTOR3 GetMinBound() { return m_minBound; }
	D3DXVECTOR3 GetMaxBound() { return m_maxBound; }
	float GetWidth(){ return m_width; }
	float GetHeight(){ return m_height; }
	float GetDepth(){ return m_depth; }

};
#endif // !BOUNDINGBOX_H
