#ifndef FLOOR_H
#define FLOOR_H
#include <d3d9.h>
#include "GameObject.h"
class Floor : public GameObject
{
private :
	D3DXVECTOR3 m_bottomLeft;
	D3DXVECTOR3 m_topRight;
	float m_width;
	float m_height;
	float m_depth;
	bool ComputeBoundingBox();
	bool m_computedBoundingBox;
	bool m_isOccupied;
public:
	Floor(Mesh* mesh);
	~Floor(){}
	void Update(float timestep){}
	float GetWidth();
	float GetHeight();
	float GetDepth();
	bool IsOccupied() { return m_isOccupied; }
	void SetOccupied(bool occupied) { m_isOccupied = occupied; }
	
};
#endif // !FLOOR_H
