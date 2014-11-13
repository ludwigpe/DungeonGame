#ifndef GAMECOLLIDER_H
#define GAMECOLLIDER_H
#include "BoundingBox.h"
#include <d3d9.h>
class GameObject;
class Renderer;
/*
	This is an abstract class for a GameCollider. A game object that wants 
	to use a GameCollider must ovveride a couple of functions
*/

class GameCollider
{
protected:
	BoundingBox* m_boundingBox;		// the bounding box describing this collider
	D3DXVECTOR3 m_colliderPosition;		// the position of the collider
public:
	GameCollider();
	~GameCollider();
	virtual void OnCollision(GameObject* other) = 0;
	virtual bool SetUpCollider() = 0;
	virtual D3DXVECTOR3 GetMaxBound() = 0;			
	virtual D3DXVECTOR3 GetMinBound() = 0;
	virtual GameObject* GetGameObject() = 0;
	virtual void DrawBounds(Renderer* renderer);
	BoundingBox* GetBoundingBoxLocal() { return m_boundingBox; }
	//std::vector<D3DXVECTOR3> GetBoundingBox();
};
#endif // !GAMECOLLIDER_H
