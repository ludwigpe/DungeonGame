#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#define COOLDOWN 1.0f

enum EnemyType
{
	ENEMY_CERBERUS,
	ENEMY_BOSS,
};

class Enemy : public Character
{
protected:
	float m_cooldown;
public:
	Enemy(Mesh* mesh, float maxHealth, float health);
	Enemy(EnemyType type);
	Enemy(float maxHealth);
	void Update(float timestep);

	// collider interface
	void OnCollision(GameObject* other);
	D3DXVECTOR3 GetRandomTarget();
	GameObject* GetGameObject() { return this;}

};
#endif // !ENEMY_H
