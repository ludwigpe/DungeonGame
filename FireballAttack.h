#ifndef FIREBALLATTACK_H
#define FIREBALLATTACK_H
#include "BattleAttack.h"
/*
	this class will not do anything specifically yet but could be extended to
	maybe burn an enemy and decrese its healt over time. But right now it 
	does the same as a reguallar BattleAttack due to time constraints
*/
class Player;
class Enemy;
class FireballAttack : public BattleAttack
{
private:
	const char* m_modelName;
public:
	FireballAttack();
	void OnHit(GameObject* other);
	//void OnSuccess(Player* p);
	//void OnSuccess(Enemy* e);
	const char* GetModelName() { return m_modelName; }
};
#endif // !FIREBALLATTACK_H
