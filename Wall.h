#ifndef WALL_H
#define WALL_H
#include "GameObject.h"
#include "GameCollider.h"
enum WallDirection
{
	WALL_VERTICAL,
	WALL_HORIZONTAL,
};
class Wall : public GameObject, public GameCollider
{
private :
	WallDirection m_wallType;
	std::vector<std::pair<D3DXVECTOR3, D3DXVECTOR3>> m_sides;
	D3DXVECTOR3 m_wallMaxBound;	//the max bound for the cube wall
	D3DXVECTOR3 m_wallMinBound;	// the min boind for the cube wall
public:
	Wall(Mesh* mesh, D3DXVECTOR3 pos);
	~Wall();
	void Update(float timestep){}
	void OnCollision(GameObject* other){}
	void Draw(Renderer* renderer);
	bool SetUpCollider();
	D3DXVECTOR3 GetMaxBound();
	D3DXVECTOR3 GetMinBound();
	GameObject* GetGameObject(){ return this; }
	void SetPos(D3DXVECTOR3 newPos);
};


#endif // !WALL_H
