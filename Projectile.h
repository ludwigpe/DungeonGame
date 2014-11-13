#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObject.h"
#include "GameCollider.h"
#include "BattleAttack.h"
#define DEFAULT_TTL 3.0f
/*
	A projectile travels in a give direction until it either exceedes its time to live
	or it collides with another object, except another projectile.
	A projectile also has a battle attack attached to it which will be called for when 
	the projectile collides with another object.
*/

class Projectile : public GameObject, public GameCollider
{
private:
	float m_timetoLive;
	bool m_visible;
	//float m_speed;
	BattleAttack* m_bAttack;
public:
	Projectile(float speed, D3DXVECTOR3 pos, D3DXVECTOR3 direction);
	Projectile(float speed, D3DXVECTOR3 pos, D3DXVECTOR3 direction, Mesh* mesh);
	~Projectile(){}
	
	void SetBattleAttack(BattleAttack* bAttack) { m_bAttack = bAttack; }
	BattleAttack* GetBattleAttack() { return m_bAttack; }

	virtual void Update(float timestep);
	virtual void Draw(Renderer* renderer);
	virtual void OnCollision(GameObject* other);
	virtual D3DXVECTOR3 GetMaxBound();
	virtual D3DXVECTOR3 GetMinBound();
	virtual bool SetUpCollider();
	virtual GameObject* GetGameObject() { return this;}
};
#endif // !PROJECTILE_H
