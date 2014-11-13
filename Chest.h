#ifndef CHEST_H
#define CHEST_H

#include "GameObject.h"
#include "GameCollider.h"


class Chest : public GameObject, public GameCollider
{
private:
	int m_hpReward;
public:
	Chest(int reward);
	~Chest(){}
	void OnCollision(GameObject* other);
	bool SetUpCollider();
	D3DXVECTOR3 GetMaxBound();
	D3DXVECTOR3 GetMinBound();
	GameObject* GetGameObject(){ return this; }
	void Draw(Renderer* renderer);

	void Update(float timestep);
};
#endif // !CHEST_H
